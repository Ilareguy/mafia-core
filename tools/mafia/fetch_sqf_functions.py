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
from os.path import isfile
from urllib.error import URLError
import json
from .sqf_utils import validate_name

CACHE_DIRECTORY = 'cache/'
CACHE_FUNCTION_DIRECTORY = CACHE_DIRECTORY + 'functions/'
BI_WIKI_URL = 'https://community.bistudio.com/wikidata/api.php?'
SQF_FUNCTIONS = 'action=parse&pageid=12689&format=json&prop=links'
SQF_FUNCTIONS_CACHED_FILE = CACHE_DIRECTORY + 'sqf_functions.json'


class SQFFunction(object):
    def __init__(self, name, parameters=None):
        self.name = name
        if parameters is None:
            parameters = {}


def fetch_sqf_functions():
    functions = {}

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

    # Parse each function. This may be time-consuming on first runs
    for function_object in functions.values():
        function_cached_path = CACHE_FUNCTION_DIRECTORY + function_object.name + '.json'

        if not isfile(function_cached_path):
            # File not cached yet; do it now
            function_url = BI_WIKI_URL + f'action=parse&page={function_object.name}&format=json&prop=text'
            try:
                print(f'No cached data for {function_object.name}; fetching now... ')
                with urllib.request.urlopen(function_url) as response:
                    with open(function_cached_path, "w+b") as file:
                        file.write(response.read())
                    print('Fetch successful.')
            except URLError as e:
                print(f'Could not fetch data for function {function_object.name} from wiki. Exception: {e}')
                continue

        try:
            # Get a handle to cached data
            with open(function_cached_path) as function_file_handle:
                data = json.load(function_file_handle)
                print(f'Successfully loaded JSON data for function {function_object.name}!')
        except OSError as e:
            print(f'Could not open "{function_cached_path}" for reading. Exception: {e}')
            continue


if __name__ == '__main__':
    fetch_sqf_functions()
