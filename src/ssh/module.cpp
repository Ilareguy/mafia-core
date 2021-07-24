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
 * File created by Anthony Ilareguy on 2021-07-22.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "module.h"
#include "../ssh_server.h"

using namespace mafia;

ssh::ModuleInterface::ModuleInterface():
        Command("module", "Module-related interface (load, unload, reload, list, ...)") {}

ssh::ModuleInterface::~ModuleInterface() = default;

std::string ssh::ModuleInterface::execute_command(
        const std::string& command_name,
        const cxxopts::ParseResult& args,
        ::mafia::SSHServer& ssh_server
)
{
    if(args.count("load"))
    {
        // Load a module
        auto module_name = args["load"].as<std::string>();

        schedule([&module_name](){
            //
        }, THREAD_MAIN);

        return fmt::format("Loading module \"{}\"...", module_name);
    }

    return "<not implemented>";
}

void ssh::ModuleInterface::init_ssh_interface(cxxopts::OptionAdder&& opts)
{
    opts
            ("l,load", "Load a module", cxxopts::value<std::string>())
            ("u,unload", "Unload a module", cxxopts::value<std::string>())
            ("r,reload", "Reloads a loaded module", cxxopts::value<std::string>())
            ("list", "Lists the currently loaded modules")
        /*("help", "Shows this help message")*/;
}
