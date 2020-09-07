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
BI_WIKI_URL = 'https://community.bistudio.com/wikidata/api.php?'
SQF_FUNCTIONS = 'action=parse&pageid=12689&format=json&prop=links'
SQF_FUNCTIONS_CACHED_FILE = CACHE_DIRECTORY + 'sqf_functions.json'


def fetch_sqf_functions():
    if not isfile(SQF_FUNCTIONS_CACHED_FILE):
        # Cached data doesn't exist yet; fetch now
        try:
            with urllib.request.urlopen(BI_WIKI_URL + SQF_FUNCTIONS) as response:
                with open(SQF_FUNCTIONS_CACHED_FILE, "w+b") as file:
                    file.write(response.read())
        except URLError as e:
            print(f'Could not fetch fresh data from wiki. Exception: {e}')
            return

    try:
        # Get a handle to cached data
        with open(SQF_FUNCTIONS_CACHED_FILE) as sqf_functions_file:
            data = json.load(sqf_functions_file)

    except OSError as e:
        print(f'Could not open "{SQF_FUNCTIONS_CACHED_FILE}" for reading. Exception: {e}')
        return

    # Parse links
    for link in data['parse']['links']:
        if link['ns'] == 0 and validate_name(link['*']):
            print(link['*'] + '\n')


if __name__ == '__main__':
    fetch_sqf_functions()
