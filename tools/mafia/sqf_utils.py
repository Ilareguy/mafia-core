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
from json import JSONEncoder
from os.path import isfile
from typing import Optional, Generator, List
from urllib.error import URLError
from lxml import etree
from io import StringIO

# @TODO: Parsing params for `remoteExec` and other commands not working.

##################################################
# Consts
##################################################
SQF_COMMAND_NAME_REGEX = '^[a-zA-Z0-9_][a-zA-Z0-9_]{2,}$'
CACHE_DIRECTORY = 'cache/'
CACHE_COMMAND_DIRECTORY = CACHE_DIRECTORY + 'commands/'
SQF_COMMANDS_STRUCTURE_CACHED_FILE = 'commands.json'
SQF_COMMANDS_RAW_CACHED_FILE = CACHE_DIRECTORY + 'sqf_commands_raw.json'
BI_WIKI_URL = 'https://community.bistudio.com/wikidata/api.php'

with open('command_overrides.json') as overrides_file:
    COMMAND_OVERRIDES = json.load(overrides_file)

with open('command_ignores.json') as ignores_file:
    COMMAND_IGNORES = json.load(ignores_file)
    COMMAND_IGNORES = [x.upper() for x in COMMAND_IGNORES]

SQF_COMMAND_PARAM_REGEX = re.compile(
    r"^(?P<name>[a-zA-B0-9]+): *\[*(?P<type>[a-zA-Z]+)\]+ *(- ){0,1}(?P<description>.*)$",
    re.IGNORECASE
)
SQF_COMMAND_RETURNS_REGEX = re.compile(
    r"^\[{0,2}(?P<type>[a-zA-B0-9]+)\]{0,2}",
    re.IGNORECASE
)


##################################################
# Classes
##################################################
class SQFCommand(object):
    def __init__(
            self,
            name=None,
            description=None,
            introduced_version=None,
            syntax=None,
            parameters=None,
            returns=None
    ):
        self.name = name
        self.description = description
        self.introduced_version = introduced_version
        self.syntax = syntax
        self.parameters: List[SQFCommandParameter] = parameters
        self.returns: SQFCommandReturn = returns

        # Fill overridden data, if any
        self._find_overrides()

        # Check for empty parameters
        if self.description == "" or self.description is None:
            print(f"Command `{self.name}` has empty description.")

        if self.syntax == "" or self.syntax is None:
            print(f"Command `{self.name}` has empty syntax.")

        if self.parameters is None:
            # This is a problem! Empty parameters (i.e. `[]`) is fine; but None means no data could be fetched.
            print(f"Command `{self.name}` has no parameters data!")
            self.parameters = []

        if self.returns is None:
            # This is a problem! Must be of type `SQFCommandReturn`. None means it couldn't be parsed.
            print(f"Command `{self.name}` has no return type specified. Assuming `Nothing`.")
            self.returns = SQFCommandReturn()

    def _find_overrides(self):
        try:
            overrides_detail = COMMAND_OVERRIDES[self.name]
            print(f"Overriding data for command \"{self.name}\".")
            for key, value in overrides_detail.items():
                if key == "parameters":
                    params = []
                    for param in value:
                        params.append(SQFCommandParameter(name=param['name'],
                                                          description=param['description'],
                                                          sqf_type=param['sqf_type']))
                    setattr(self, key, params)
                elif key == 'returns':
                    setattr(self, key, SQFCommandReturn(description=value['description'], sqf_type=value['sqf_type']))
                else:
                    setattr(self, key, value)
        except KeyError:
            pass  # No override data for this command

    class JSONEncoder(JSONEncoder):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

        def default(self, obj):
            param_encoder = SQFCommandParameter.JSONEncoder()
            return_encoder = SQFCommandReturn.JSONEncoder()
            params_out = []
            returns = return_encoder.default(obj.returns)

            for p in obj.parameters:
                params_out.append(param_encoder.default(p))

            return {
                'name': obj.name,
                'description': obj.description,
                'introduced_version': obj.introduced_version,
                'syntax': obj.syntax,
                'parameters': params_out,
                'returns': returns
            }


class SQFCommandReturn(object):
    def __init__(self, description=None, sqf_type="Nothing"):
        self.description = description
        self.sqf_type = sqf_type
        if self.sqf_type is not None:
            if self.sqf_type.casefold() == "bool":
                self.sqf_type = "Boolean"
            elif self.sqf_type.casefold().startswith("position"):
                self.sqf_type = self.sqf_type.capitalize()  # Just make sure the case is correct
            elif self.sqf_type.casefold().startswith("hashmap"):
                self.sqf_type = self.sqf_type.capitalize()  # Just make sure the case is correct
        else:
            self.sqf_type = "Nothing"

    class JSONEncoder(JSONEncoder):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

        def default(self, obj):
            return {
                'description': obj.description,
                'sqf_type': obj.sqf_type
            }


class SQFCommandParameter(object):
    def __init__(self, name=None, description=None, sqf_type=None):
        self._name: str = name
        self.description: str = description
        self.sqf_type: str = sqf_type

        if self.sqf_type.casefold() == "bool":
            self.sqf_type = "Boolean"
        elif self.sqf_type.casefold().startswith("position"):
            self.sqf_type = self.sqf_type.capitalize()  # Just make sure the case is correct
        elif self.sqf_type.casefold().startswith("hashmap"):
            self.sqf_type = self.sqf_type.capitalize()  # Just make sure the case is correct

    @property
    def name(self) -> str:
        if self._name.casefold().startswith('2'):
            return "mode"
        return self._name

    class JSONEncoder(JSONEncoder):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

        def default(self, obj):
            try:
                return {
                    'name': obj.name,
                    'description': obj.description,
                    'sqf_type': obj.sqf_type
                }
            except AttributeError:
                pass


class UnknownSQFCommand(SQFCommand):
    """
    Some SQF commands have no entry in the wiki. In such cases, the SQFCommands object will contain an instance of
    this class instead of SQFCommand.
    """

    def __init__(self, name):
        super().__init__(name)


# class SQFCommands(JSONEncoder):
#     def __init__(self):
#         super().__init__()
#         self.commands = {}
#
#     def get(self, name):
#         return self.commands[name]
#
#     def add(self, name, command):
#         self.commands[name] = command
#
#     class JSONEncoder(JSONEncoder):
#         def __init__(self, *args, **kwargs):
#             super().__init__(*args, **kwargs)
#
#         def default(self, obj):
#             out = []
#             encoder = SQFCommand.JSONEncoder()
#             for f in obj.commands.values():
#                 out.append(encoder.default(f))
#             return out


##################################################
# Exceptions
##################################################
class NoWikiEntryError(Exception):
    """
    Raised when an SQF function has no usable wiki entry to parse. Such a function likely exists in the VR engine
    but we hae no data to build upon.
    """
    pass


def gen_sqf_commands() -> Generator[SQFCommand, None, None]:
    """
    Retrieves an SQFCommands instance containing structured data about all known SQF commands.
    On first runs, or after the cache is cleared, this function will make numerous web requests to retrieve fresh data
    from BI's wiki.

    Execution is done is several steps:
        First, a list of all known SQF commands is fetched and stored as "sqf_commands_raw.json" in the cache
        directory.

        Then, raw data about each command is fetched from BI's wiki and stored in the commands cache directory.

        Finally, when all that raw data is available, structured usable data is built and stored in
        "commands.json" in the cache directory.

    The function then returns a SQFCommands object containing all that data.

    :return: A SQFCommands instance.
    """

    raw_commands = []

    ##################################################
    # Fetch a list of command names
    ##################################################

    if not isfile(SQF_COMMANDS_RAW_CACHED_FILE):
        # Cached data doesn't exist yet; fetch now.

        """
        The MediaWiki API has changed since the last use of this script. At this time, we can no longer retrieve all the
        links/scripting commands from a single GET request. Instead, we have to request a list of "category members"
        that belong to the "Category:Arma_3:_Scripting_Commands" (page ID 12299) category. Each "category member" is in
        fact a wiki page describing a single Arma scripting command.
        
        This GET request limits the amount of results to an upper limit of 500, so multiple requests need to be sent in
        order to retrieve every single scripting command.
        """
        try:
            scripting_commands = []
            done = False  # True when we're done fetching all the category members
            cmcontinue = False  # "cmcontinue" string provided by the MediaWiki API on each call

            while not done:
                url = BI_WIKI_URL + "?action=query&list=categorymembers&cmpageid=12299&cmlimit=max&cmprop=title|ids" \
                                    "&format=json"
                if cmcontinue is not False:
                    url += f"&cmcontinue={cmcontinue}"
                with urllib.request.urlopen(url) as response:
                    data = json.loads(response.read())
                    try:
                        cmcontinue = data['continue']['cmcontinue']
                    except KeyError:
                        done = True
                    scripting_commands.extend(data['query']['categorymembers'])

                    del data

            with open(SQF_COMMANDS_RAW_CACHED_FILE, "w+") as scripting_commands_file:
                json.dump(scripting_commands, scripting_commands_file)

            del scripting_commands

        except URLError as e:
            print(f'Could not fetch commands list from wiki. Exception: {e}')
            return None

    try:
        # Get a handle to cached data
        with open(SQF_COMMANDS_RAW_CACHED_FILE) as sqf_commands_file:
            data = json.load(sqf_commands_file)

    except OSError as e:
        print(f'Could not open "{SQF_COMMANDS_RAW_CACHED_FILE}" for reading. Exception: {e}')
        return None

    # Parse command names
    for link in data:
        command_name = link['title']
        # There are some edge-cases in the commands list
        if command_name == 'setPylonLoadOut':
            command_name = 'setPylonLoadout'
        elif command_name == 'setWantedRpmRTD':
            command_name = 'setWantedRPMRTD'

        if link['ns'] == 0 and validate_sqf_command_name(command_name):
            raw_commands.append(command_name)

    del data

    ##################################################
    # Build structured data about each command.
    # This may be time-consuming on first runs
    ##################################################
    for command_name in raw_commands:
        if command_name.upper() in COMMAND_IGNORES:
            print(f"Ignoring command \"{command_name}\".")
            continue  # Do not process this command

        command_raw_cache_path = CACHE_COMMAND_DIRECTORY + command_name.upper() + '.json'

        try:
            if not isfile(command_raw_cache_path):
                # Fetch and cache raw data about this command
                command_url = BI_WIKI_URL + f'?action=parse&page={command_name}&format=json&prop=parsetree'
                print(f'No cached data for {command_name}; fetching now... ')
                with urllib.request.urlopen(command_url) as response:
                    with open(command_raw_cache_path, "w+b") as file:
                        file.write(response.read())
                    print('Fetch successful.')

        # Can't open wiki article
        except URLError as e:
            print(f'Could not fetch data for command {command_name} from wiki. Exception: {e}')
            continue

        # Can't open cache file
        except OSError as e:
            print(f'Could not open "{command_raw_cache_path}" for writing. Exception: {e}')
            continue

        # We have raw data. Now build something useful
        try:
            with open(command_raw_cache_path) as command_raw_cache_file_handle:
                # structured_commands.add(command_name,
                #                        _parse_command_data(command_name, command_raw_cache_file_handle))
                yield _parse_command_data(command_name, command_raw_cache_file_handle)

        except OSError as e:
            print(f'Could not open "{command_raw_cache_path}" for reading. Exception: {e}')
            # structured_commands.add(command_name, UnknownSQFCommand(command_name))
            continue

        except NoWikiEntryError:
            print(f'Command \"{command_name}\" has no entry in the wiki!')
            # structured_commands.add(command_name, UnknownSQFCommand(command_name))
            continue

    # with open(SQF_COMMANDS_STRUCTURE_CACHED_FILE, "w") as out:
    #     json.dump(structured_commands, out, cls=SQFCommands.JSONEncoder, indent=4)
    #
    # return structured_commands


def _parse_command_data(name, file):
    # Load raw data
    command_raw_data = json.load(file)

    if 'error' in command_raw_data:
        # This command has no valid entry in the wiki, and therefore no usable data for us.
        raise NoWikiEntryError()

    # Fetch command data
    xtree = etree.parse(StringIO(command_raw_data['parse']['parsetree']['*']))
    returns = _parse_return_details(xtree)

    return SQFCommand(
        name=name,
        description=_parse_description(xtree),
        syntax=_parse_syntax(xtree),
        parameters=_parse_parameters(xtree),
        returns=returns
    )


def _parse_description(xtree):
    try:
        return xtree.xpath('//root/template/part[name[@index="3"]]/value/text()')[0].strip()
    except IndexError:
        pass

    try:
        return xtree.xpath('//root/template/part[name[text()[contains(.,\'descr\')]]]/value/text()')[0].strip()
    except IndexError:
        pass

    return None


def _parse_syntax(xtree):
    try:
        return xtree.xpath('//root/template/part[name[@index="4"]]/value/text()')[0].strip()
    except IndexError:
        pass

    try:
        return xtree.xpath('//root/template/part[name[text()[contains(.,\'s1\')]]]/value/text()')[0].strip()
    except IndexError:
        pass

    return None


def _parse_parameters(xtree):
    class NotFound(BaseException):
        pass

    def xpath():
        try:
            return xtree.xpath('//root/template/part[name[text()[contains(.,\'p\')]]]/value/text()')
        except IndexError:
            pass

        # Additional xpath()s can be added here later to match more specific cases.

        raise NotFound

    # Compile regex if necessary
    global SQF_COMMAND_PARAM_REGEX

    # Acquire raw data from XPath
    try:
        params_raw = xpath()
    except NotFound:
        return None

    # Remove empty data returned by XPath
    params_raw = _strip_and_remove_empties(params_raw)

    # Parse individual details
    params = []
    for raw_param in params_raw:
        data = SQF_COMMAND_PARAM_REGEX.search(raw_param)

        # No data?
        if data is None:
            continue

        param_name: str = str(data.group("name"))

        # Duplicate param?
        if param_name.casefold() in [c.name.casefold() for c in params]:
            continue

        params.append(SQFCommandParameter(
            name=param_name,
            description=data.group("description"),
            sqf_type=data.group("type")
        ))

    return params


def _parse_return_details(xtree):
    class NotFound(BaseException):
        pass

    def xpath():
        try:
            return xtree.xpath('//root/template/part[name[@index="5"]]/value/text()')[0]
        except IndexError:
            pass

        try:
            return xtree.xpath('//root/template/part[name[text()[starts-with(.,\'r1\')]]]/value/text()')[0]
        except IndexError:
            pass

        try:
            return xtree.xpath('//root/template/part[name[@index="1"]]/value/text()')[0]
        except IndexError:
            pass

        # Additional xpath()s can be added here later to match more specific cases.

        raise NotFound

    # Compile regex if necessary
    global SQF_COMMAND_RETURNS_REGEX

    # Acquire raw data from XPath
    try:
        returns_raw = xpath()
    except NotFound:
        return None
    returns_raw = returns_raw.strip()

    # Parse type and description
    data = SQF_COMMAND_RETURNS_REGEX.search(returns_raw)

    return SQFCommandReturn(description=returns_raw, sqf_type=None if data is None else data.group('type'))


def validate_sqf_command_name(name):
    # Evaluates whether the given name is a valid SQF command name.
    return re.match(SQF_COMMAND_NAME_REGEX, name) is not None


def _strip_and_remove_empties(arr):
    r = []
    for entry in arr:
        stripped = entry.strip()
        if stripped != "":
            r.append(stripped)
    return r
