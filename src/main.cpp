/********************************************************
 *
 *             ______  ____    ______   ______
 *     /'\_/`\/\  _  \/\  _`\ /\__  _\ /\  _  \
 *    /\      \ \ \L\ \ \ \L\_\/_/\ \/ \ \ \L\ \
 *    \ \ \__\ \ \  __ \ \  _\/  \ \ \  \ \  __ \
 *     \ \ \_/\ \ \ \/\ \ \ \/    \_\ \__\ \ \/\ \
 *      \ \_\\ \_\ \_\ \_\ \_\    /\_____\\ \_\ \_\
 *       \/_/ \/_/\/_/\/_/\/_/    \/_____/ \/_/\/_/
 *
 *                  The Mafia project
 *
 ********************************************************
 *
 * File created by Anthony Ilareguy on 28/02/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "memory_utility.h"
#include "shared.h"
#include "arguments.h"
#include <fmt/format.h>
#include <optional>
#include <string_view>

// See https://community.bistudio.com/wiki/callExtension
// See https://community.bistudio.com/wiki/Extensions
extern "C"
{
//--- Engine called on extension load
DLLEXPORT void __stdcall RVExtensionVersion(char*, int);

//--- STRING callExtension STRING
DLLEXPORT void __stdcall RVExtension(char*, int, const char*);

//--- STRING callExtension ARRAY
//DLLEXPORT int __stdcall RVExtensionArgs(char*, int, const char*, const char**, int);
}

namespace mafia::_private
{
    bool initialized {false};
}

std::optional<std::string_view> get_command(std::string_view input)
{
    size_t cmd_end = input.find(':');
    if (cmd_end < 1)
    {
        return {};
    }

    return input.substr(0, cmd_end);
}

void __stdcall RVExtensionVersion(char* output, int outputSize)
{
    const std::string version_str = fmt::format(
            "{}.{}.{}{}",
            0,
            0,
            1,
            "-dev"
    );
    strncpy_s(output, outputSize, version_str.c_str(), _TRUNCATE);
    output[outputSize - 1] = 0x00;
}

/**
 * Function invoked by Arma when a mod calls in SQF:
 *     "mafia" callExtension ("function:arguments");
 */
void __stdcall RVExtension(char* output, int outputSize, const char* function)
{
    using namespace std::literals; // operator""sv

    std::string_view input = function;
    std::optional<std::string_view> cmd = get_command(input);

    /**
     * Note: output[outputSize-1] must always be 0x00. Arma checks for this and if it's not the case, then a "buffer
     * overrun" message is displayed and Arma intentionally dies. See https://feedback.bistudio.com/T82390
     */

    if (!cmd)
    {
        output[0] = 0x00;
        output[outputSize - 1] = 0x00;
        return;
    }

    std::string_view command = cmd.value();
    std::string argument_str;
    if (command.length() > 1 && input.length() > command.length() + 1)
    {
        argument_str = input.substr(command.length() + 1, input.length() + 1 - command.length());
    }
    mafia::Arguments _args(argument_str);
    std::string result = "-1";

    if (command == "init"sv)
    {
        mafia::memory_utility::init();
        //mafia::logger()->info("Mafia Core DLL initialized.");
        mafia::_private::initialized = true;

        output[outputSize - 1] = 0x00;
        return;
    }
    else if (command == "echo"sv)
    {
        result = function;
    }
    else if (command == "init_patch"sv)
    {
        //uintptr_t game_state_addr = mafia::Loader::find_game_state(
        //        reinterpret_cast<uintptr_t>(output) + outputSize
        //);
        //mafia::controller().get_loader().do_function_walk(game_state_addr);
        output[outputSize - 1] = 0x00;
        return;
    }

    if (!mafia::_private::initialized)
    {
        // If Mafia wasn't initialized, we can't execute other commands
        snprintf(
                output, outputSize, "%s",
                fmt::format("Called Mafia extension with \"{}\", but Mafia wasn't initialized.", function).c_str());
        output[outputSize - 1] = 0x00;
        return;
    }

    //mafia::logger()->info(R"({}: Received command: "{}" with args: "{}")", __FUNCTION__, command, argument_str);
    result = fmt::format(R"({}: Received command: "{}" with args: "{}")", __FUNCTION__, command, argument_str);

    //result = mafia::controller().rv_call(command, _args);
    if (result.length() > 0)
    { snprintf(output, outputSize, "%s", result.c_str()); }
    output[outputSize - 1] = 0x00;
}
