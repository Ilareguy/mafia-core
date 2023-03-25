import os
import traceback
import json

from . import sqf_utils
from re import sub


def camel_case(s: str):
    s = sub(r"(_|-)+", " ", s).title().replace(" ", "")
    return ''.join([s[0].lower(), s[1:]])


def pascal_case(s: str):
    ret = camel_case(s)
    return f"{ret[0].upper()}{ret[1:]}"


def snake_case(s):
    return '_'.join(
        sub('([A-Z][a-z]+)', r' \1',
            sub('([A-Z]+)', r' \1',
                s.replace('-', ' '))).split()).lower()


def gen_proto():
    args_data = ""
    ret_data = ""
    data = ""

    data += """syntax = "proto3";
import "types.proto";
package mafia.sqf;

service SQF {
"""

    for idx, command in enumerate(sqf_utils.gen_sqf_commands()):
        command: sqf_utils.SQFCommand

        # if idx == 40:
        #     break

        # Generate the parameters class, if necessary
        params = command.parameters
        if params is None or len(params) == 0:
            args_cls = "mafia.types.NoArgs"
        else:
            args_cls = f"{f'{command.name[0].upper()}{command.name[1:]}'}Args"
            args_data += f"""message {args_cls} {{\n"""
            for p_idx, p in enumerate(params, start=1):
                p: sqf_utils.SQFCommandParameter
                args_data += f"\tmafia.types.RV{pascal_case(p.sqf_type)} {snake_case(p.name)} = {p_idx};\n"
            args_data += "};\n\n"

        # Generate the return class, if necessary
        ret_cls = f"{f'{command.name[0].upper()}{command.name[1:]}'}Ret"
        ret_data += f"""message {ret_cls} {{\n"""
        ret_data += f"""\tmafia.types.RV{command.returns.sqf_type} data = 1;\n"""
        ret_data += "};\n\n"

        # params = command.parameters
        # if params is None or len(params) == 0:
        #     gen_params = "mafia.types.NularArgs"
        # elif len(params) == 1:
        #     gen_params = "mafia.types.UnaryArgs"
        # elif len(params) == 2:
        #     gen_params = "mafia.types.BinaryArgs"
        # else:
        #     print(f"Got too many arguments ({len(params)}) for function `{command.name}`!")
        #     print("Skipping function.")
        #     continue

        data += f"""\trpc {command.name} ({args_cls}) returns ({ret_cls});\n"""
        # print(command.name)
        # print([f"{p.name} -> {p.sqf_type}" for p in command.parameters])

    data += """}"""

    with open('dist/sqf.proto', 'w') as out:
        out.write(data)
        out.write('\n\n')
        out.write(args_data)
        out.write('\n\n')
        out.write(ret_data)


def main():
    gen_proto()


if __name__ == '__main__':
    main()
