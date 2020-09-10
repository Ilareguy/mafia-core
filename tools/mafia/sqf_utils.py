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
# [File Description]
#
#########################################################
# 
# [Credits]
#
#########################################################

import re

SQF_FUNCTION_NAME_REGEX = '^[a-zA-Z0-9_][a-zA-Z0-9_]{2,}$'


def validate_name(name):
    # Evaluates whether or not the given name is a valid SQF function name.
    return re.match(SQF_FUNCTION_NAME_REGEX, name) is not None
