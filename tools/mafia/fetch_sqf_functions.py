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

from .sqf_utils import SQFCommand, fetch_sqf_commands
import urllib.request
import json


if __name__ == '__main__':
    functions = fetch_sqf_commands()
    exit(0)

    with urllib.request.urlopen("https://community.bistudio.com/wikidata/api.php?action=parse&page=set3DENMissionAttribute&format=json&prop=parsetree") as response:
        with open("output.json", "w+b") as file:
            file.write(response.read())

    with open("output.json") as sqf_functions_file:
        data = json.load(sqf_functions_file)
        with open("output.xml", "w+") as file:
            file.write(data['parse']['parsetree']['*'])
