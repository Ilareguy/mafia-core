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

#include "mafia.h"
#include "mafia_private.h"
#include "memory_utility.h"
#include "rv_controller.h"
#include "logging.h"
#include "logging_private.h"
#include "shared.h"
#include "arguments.h"
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
    sprintf_s(
            output, outputSize,
            fmt::format(
                    "v.{}.{}.{}{}",
                    mafia::version::major,
                    mafia::version::minor,
                    mafia::version::revision,
                    mafia::version::suffix
            )
                    .c_str());
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
        sprintf_s(output, outputSize, "Mafia did not receive any command.");
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
    if (mafia::_private::initialized)
    {
        mafia::log::debug(R"(Mafia received command: "{}" with args: "{}")", command, argument_str);
    }

    if (command == "init"sv)
    {
        if (!mafia::_private::initialized)
        {
            mafia::log::_private::init();
            mafia::_private::initialized = true;
        }

        sprintf_s(output, outputSize, "Mafia DLL initialized.");
        mafia::log::info("Mafia DLL initialized.");
        output[outputSize - 1] = 0x00;
        return;
    }
    else if (command == "init_patch"sv)
    {
        mafia::_private::init_controller(reinterpret_cast<uintptr_t>(output) + outputSize);
        mafia::log::info("Controller initialized.");
        //output[outputSize - 1] = 0x00;
        return;
    }

    result = mafia::controller()->rv_call(command, _args);
    if (result.length() > 0)
    { sprintf_s(output, outputSize, result.c_str()); }
    output[outputSize - 1] = 0x00;
}
