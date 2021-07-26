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
 * File created by Anthony Ilareguy on 2021-07-26.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "javascript.h"
#include "../mafia.h"
#include "../rv_controller.h"
#include "../runtime.h"

using namespace mafia;

ssh::JavascriptInterface::JavascriptInterface():
        Command("js", "Run Javascript code") {}

ssh::JavascriptInterface::~JavascriptInterface() = default;

std::string ssh::JavascriptInterface::execute_command(
        const std::string& command_name,
        const cxxopts::ParseResult& args,
        ::mafia::SSHServer& ssh_server
)
{
    if (args.count("code"))
    {
        auto js_code = args["code"].as<std::string>();
        char* eval_c = ::mafia::controller()->javascript_runtime().eval(js_code.c_str());
        auto eval_result = std::string {eval_c};
        delete[] eval_c;
        return eval_result;
    }

    return "Usage: `js -c \"hello_world();\"`";
}

void ssh::JavascriptInterface::init_ssh_interface(cxxopts::OptionAdder&& opts, cxxopts::Options& options)
{
    opts
            ("c,code", "The Javascript code to run", cxxopts::value<std::string>());
}
