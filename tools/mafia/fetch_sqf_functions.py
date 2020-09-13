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

from .sqf_utils import SQFFunction, fetch_sqf_functions


if __name__ == '__main__':
    def on_message(message):
        print(message)

    functions = fetch_sqf_functions(on_message)
