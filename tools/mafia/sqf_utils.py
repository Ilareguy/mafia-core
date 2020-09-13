#!/usr/bin/env python3
#########################################################
#
#             ______  ____    ______   ______
#     /'\_/`\/\  _  \/\  _`\ /\__  _\ /\  _  \
#    /\      \ \ \L\ \ \ \L\_\/_/\ \/ \ \ \L\ \
#    \ \ \__\ \ \  __ \ \  _\/  \ \ \  \ \  __ \
#     \ \ \_/\ \ \ \/\ \ \ \/    \_\ \__\ \ \/\ \
#      \ \_\\ \_\ \_\ \_\ \_\    /\_____\\ \_\ \_\
#       \/_/ \/_/\/_/\/_/\/_/    \/_____/ \/_/\/_/
#
#                  The Mafia project
#
#########################################################
#
# File created by Anthony Ilareguy on 2020-09-07.
# Utility classes and functions to deal with SQF function metadata.
#
#########################################################

import re
import urllib.request
import json
from io import StringIO
from json import JSONEncoder
from os.path import isfile
from urllib.error import URLError
from lxml import etree

##################################################
# Consts
##################################################
SQF_FUNCTION_NAME_REGEX = '^[a-zA-Z0-9_][a-zA-Z0-9_]{2,}$'
CACHE_DIRECTORY = 'cache/'
CACHE_FUNCTION_DIRECTORY = CACHE_DIRECTORY + 'functions/'
SQF_FUNCTIONS_STRUCTURE_CACHED_FILE = CACHE_DIRECTORY + 'sqf_functions_structured.json'
SQF_FUNCTIONS_RAW_CACHED_FILE = CACHE_DIRECTORY + 'sqf_functions_raw.json'
BI_WIKI_URL = 'https://community.bistudio.com/wikidata/api.php?'
SQF_FUNCTIONS = 'action=parse&pageid=12689&format=json&prop=links'

##################################################
# Lazy-initialized
##################################################
_sqf_function_parameters_regex = None


##################################################
# Classes
##################################################
class SQFFunction(object):
    def __init__(self, name=None, description=None, introduced_version=None, syntax=None, parameters=None):
        self.name = name
        self.description = description
        self.introduced_version = introduced_version
        self.syntax = syntax
        self.parameters = parameters
        if self.parameters is None:
            self.parameters = []

    class JSONEncoder(JSONEncoder):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

        def default(self, obj):
            param_encoder = SQFFunctionParameter.JSONEncoder()
            params_out = []

            for p in obj.parameters:
                params_out.append(param_encoder.default(p))

            return {
                'name': obj.name,
                'description': obj.description,
                'introduced_version': obj.introduced_version,
                'syntax': obj.syntax,
                'parameters': params_out
            }


class SQFFunctionParameter(object):
    def __init__(self, name=None, description=None, sqftype=None):
        self.name = name
        self.description = description
        self.sqftype = sqftype

    class JSONEncoder(JSONEncoder):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

        def default(self, obj):
            return {
                'name': obj.name,
                'description': obj.description,
                'sqftype': obj.sqftype
            }


class UnknownSQFFunction(SQFFunction):
    """
    Some SQF functions have no entry in the wiki. In such cases, the SQFFunctions object will contain an instance of
    this class instead of SQFFunction.
    """

    def __init__(self, name):
        super().__init__(name)


class SQFFunctions(JSONEncoder):
    def __init__(self):
        super().__init__()
        self.functions = {}

    def get(self, name):
        return self.functions[name]

    def add(self, name, function):
        self.functions[name] = function

    class JSONEncoder(JSONEncoder):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

        def default(self, obj):
            out = []
            encoder = SQFFunction.JSONEncoder()
            for f in obj.functions.values():
                out.append(encoder.default(f))
            return out


##################################################
# Exceptions
##################################################
class NoWikiEntryError(Exception):
    """
    Raised when an SQF function has no usable wiki entry to parse. Such a function likely exists in the VR engine
    but we hae no data to build upon.
    """
    pass


def fetch_sqf_functions(trace_predicate=print):
    """
    Retrieves an SQFFunctions instance containing structured data about all known SQF functions.
    On first runs, or after the cache is cleared, this function will make numerous web requests to retrieve fresh data
    from BI's wiki.

    Execution is done is several steps:
        First, a list of all known SQF functions is fetched and stored as "sqf_functions_raw.json" in the cache
        directory.

        Then, raw data about each function is fetched from BI's wiki and stored in the function cache directory.

        Finally, when all that raw data is available, structured usable data is built and stored in
        "sqf_functions_structured.json" in the cache directory.

    The function then returns a SQFFunctions object containing all that data.

    :param trace_predicate: A function to be invoked for tracing during operation. Defaults to printing messages.
    :return: A SQFFunctions instance.
    """

    raw_functions = []
    structured_functions = SQFFunctions()

    ##################################################
    # Fetch a list of function names
    ##################################################

    if not isfile(SQF_FUNCTIONS_RAW_CACHED_FILE):
        # Cached data doesn't exist yet; fetch now
        try:
            with urllib.request.urlopen(BI_WIKI_URL + SQF_FUNCTIONS) as response:
                with open(SQF_FUNCTIONS_RAW_CACHED_FILE, "w+b") as file:
                    file.write(response.read())
        except URLError as e:
            trace_predicate(f'Could not fetch functions list from wiki. Exception: {e}')
            return

    try:
        # Get a handle to cached data
        with open(SQF_FUNCTIONS_RAW_CACHED_FILE) as sqf_functions_file:
            data = json.load(sqf_functions_file)

    except OSError as e:
        trace_predicate(f'Could not open "{SQF_FUNCTIONS_RAW_CACHED_FILE}" for reading. Exception: {e}')
        return

    # Parse function names
    for link in data['parse']['links']:
        function_name = link['*']
        if link['ns'] == 0 and validate_sqf_function_name(function_name):
            raw_functions.append(function_name)

    ##################################################
    # Build structured data about each function.
    # This may be time-consuming on first runs
    ##################################################
    for function_name in raw_functions:
        function_raw_cache_path = CACHE_FUNCTION_DIRECTORY + function_name.upper() + '.json'

        try:
            if not isfile(function_raw_cache_path):
                # Fetch and cache raw data about this function
                function_url = BI_WIKI_URL + f'action=parse&page={function_name}&format=json&prop=text'
                trace_predicate(f'No cached data for {function_name}; fetching now... ')
                with urllib.request.urlopen(function_url) as response:
                    with open(function_raw_cache_path, "w+b") as file:
                        file.write(response.read())
                    trace_predicate('Fetch successful.')

        # Can't open wiki article
        except URLError as e:
            trace_predicate(f'Could not fetch data for function {function_name} from wiki. Exception: {e}')
            continue

        # Can't open cache file
        except OSError as e:
            trace_predicate(f'Could not open "{function_raw_cache_path}" for writing. Exception: {e}')
            continue

        # We have raw data. Now build something useful
        try:
            with open(function_raw_cache_path) as function_raw_cache_file_handle:
                structured_functions.add(function_name,
                                         _parse_function_data(function_name, function_raw_cache_file_handle))

        except OSError as e:
            trace_predicate(f'Could not open "{function_raw_cache_path}" for reading. Exception: {e}')
            # structured_functions.add(function_name, UnknownSQFFunction(function_name))
            continue

        except NoWikiEntryError:
            trace_predicate(f'Could not parse data about function {function_name} because it has no entry in the wiki.')
            # structured_functions.add(function_name, UnknownSQFFunction(function_name))
            continue

    with open(SQF_FUNCTIONS_STRUCTURE_CACHED_FILE, "w") as out:
        json.dump(structured_functions, out, cls=SQFFunctions.JSONEncoder, indent=4)

    return structured_functions


def _parse_function_data(name, file):
    global _sqf_function_parameters_regex
    if _sqf_function_parameters_regex is None:
        # Start by compiling SQF function param regex
        _sqf_function_parameters_regex = re.compile(r'^(?P<param_name>[a-zA-Z0-9_][a-zA-Z0-9_]+) *: *(?P<param_type>['
                                                    r'a-zA-Z0-9_\s]+) *- *(?P<param_description>.+)$')
    function_raw_data = json.load(file)

    if 'error' in function_raw_data:
        # This function has no valid entry in the wiki, and therefore no usable data for us.
        raise NoWikiEntryError()

    # Parse the entry HTML to find and parse relevant elements
    html_parser = etree.HTMLParser()
    tree = etree.parse(StringIO(function_raw_data['parse']['text']['*']), html_parser)

    # with open('test.html', 'wb') as f:
    #     f.write(etree.tostring(tree.getroot(), pretty_print=True, method="html"))

    # To test xpath, http://xpather.com/ and https://devhints.io/xpath both have proven very useful
    introduced_version = ''.join(tree.xpath('.//*[self::span][@class="release"]//text()')).strip()
    description = ''.join(tree.xpath('.//*[self::dt][text()="Description:"]/following-sibling::dd//text()')).strip()
    syntax = ''.join(tree.xpath('.//*[self::dt][text()="Syntax:"]/following-sibling::dd[1]//text()')).strip()
    parameters_tree = tree.xpath('.//*[self::dt][text()="Parameters:"]/following-sibling::dd[@class="param"]')
    parameters = []

    for i in range(len(parameters_tree)):
        # A bit slow and sketchy but works
        param_raw_text = ''.join(tree.xpath(f'.//*[self::dt][text()="Parameters:"]/following-sibling::dd'
                                            f'[@class="param"][{i + 1}]//text()')).strip()

        if param_raw_text.startswith('['):
            # Array list of right-arg names? Skip this
            continue

        m = _sqf_function_parameters_regex.search(param_raw_text)

        if m is None:
            continue

        param_name = m.group('param_name').strip()
        param_type = m.group('param_type').strip()
        param_desc = m.group('param_description')
        if param_desc is None:
            param_desc = ''
        parameters.append(SQFFunctionParameter(name=param_name, sqftype=param_type, description=param_desc))

    return SQFFunction(name,
                       description=description,
                       introduced_version=introduced_version,
                       syntax=syntax,
                       parameters=parameters)


def validate_sqf_function_name(name):
    # Evaluates whether or not the given name is a valid SQF function name.
    return re.match(SQF_FUNCTION_NAME_REGEX, name) is not None
