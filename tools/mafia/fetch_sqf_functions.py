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
#
# Fetches and parses a complete list of SQF functions from https://community.bistudio.com/
#
#########################################################

import urllib.request
import json
from os.path import isfile
from urllib.error import URLError
from .sqf_utils import validate_name
from lxml import html

CACHE_DIRECTORY = 'cache/'
CACHE_FUNCTION_DIRECTORY = CACHE_DIRECTORY + 'functions/'
BI_WIKI_URL = 'https://community.bistudio.com/wikidata/api.php?'
SQF_FUNCTIONS = 'action=parse&pageid=12689&format=json&prop=links'
SQF_FUNCTIONS_CACHED_FILE = CACHE_DIRECTORY + 'sqf_functions.json'


class SQFFunction(object):
    def __init__(self, name, description, introduced_version, parameters=None):
        self.name = name
        self.description = description
        self.introduced_version = introduced_version
        if parameters is None:
            parameters = {}


def fetch_sqf_functions():
    functions = {}

    ##################################################
    # Fetch a list of function names
    ##################################################

    if not isfile(SQF_FUNCTIONS_CACHED_FILE):
        # Cached data doesn't exist yet; fetch now
        try:
            with urllib.request.urlopen(BI_WIKI_URL + SQF_FUNCTIONS) as response:
                with open(SQF_FUNCTIONS_CACHED_FILE, "w+b") as file:
                    file.write(response.read())
        except URLError as e:
            print(f'Could not fetch functions list from wiki. Exception: {e}')
            return

    try:
        # Get a handle to cached data
        with open(SQF_FUNCTIONS_CACHED_FILE) as sqf_functions_file:
            data = json.load(sqf_functions_file)

    except OSError as e:
        print(f'Could not open "{SQF_FUNCTIONS_CACHED_FILE}" for reading. Exception: {e}')
        return

    # Parse function names
    for link in data['parse']['links']:
        function_name = link['*']
        if link['ns'] == 0 and validate_name(function_name):
            functions[function_name] = SQFFunction(function_name)

    ##################################################
    # Fetch raw details about each function
    ##################################################

    # Parse each function. This may be time-consuming on first runs
    for function_object in functions.values():
        function_raw_cache_path = CACHE_FUNCTION_DIRECTORY + function_object.name + '.json'

        if not isfile(function_raw_cache_path):
            # File not cached yet; do it now
            function_url = BI_WIKI_URL + f'action=parse&page={function_object.name}&format=json&prop=text'
            try:
                print(f'No cached data for {function_object.name}; fetching now... ')
                with urllib.request.urlopen(function_url) as response:
                    with open(function_raw_cache_path, "w+b") as file:
                        file.write(response.read())
                    print('Fetch successful.')
            except URLError as e:
                print(f'Could not fetch data for function {function_object.name} from wiki. Exception: {e}')
                continue

        try:
            # Get a handle to cached data
            with open(function_raw_cache_path) as function_raw_cache_file_handle:
                function_raw_data = json.load(function_raw_cache_file_handle)
                print(f'Successfully loaded JSON data for function {function_object.name}!')
        except OSError as e:
            print(f'Could not open "{function_raw_cache_path}" for reading. Exception: {e}')
            continue

        ##################################################
        # Parse specific details about each function
        ##################################################
        function_html_tree = html.fromstring(function_raw_data['parse']['text']['*'])
        function_object.description = function_html_tree.cssselect('dd:nth-child(2)') # first entry should be description
        function_object.introduced_version = function_html_tree.cssselect('span.release')
        function_object.parameters = function_html_tree.cssselect('dd.param') # param name
        function_object.parameters = function_html_tree.cssselect('dd.param a') # param type

        # https://docs.python-guide.org/scenarios/scrape/
        # https://lxml.de/cssselect.html
        # https://www.w3schools.com/cssref/css_selectors.asp
        # https://try.jsoup.org/


if __name__ == '__main__':
    fetch_sqf_functions()
