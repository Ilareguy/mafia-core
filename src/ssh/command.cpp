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
 * File created by Anthony Ilareguy on 2021-07-21.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "command.h"
#include "../mafia.h"
#include "../rv_controller.h"
#include <utility>

using namespace mafia;

ssh::Command::Command(std::string command_name, std::string command_description):
        _ssh_interface(std::move(command_name), std::move(command_description))
{
}

ssh::Command::~Command() = default;

std::string ssh::Command::execute(const std::string& command_name, int argc, char** argv, ::mafia::SSHServer& s)
{
    const auto parse_result = _ssh_interface.parse(argc, argv);
    return execute_command(command_name, parse_result, s);
}

void ssh::Command::init()
{
    init_ssh_interface(_ssh_interface.add_options());
}

std::string ssh::Command::help()
{
    return _ssh_interface.help();
}

void ssh::Command::schedule(ScheduledFunction_t&& function, QueuedFunctionThread target_thread)
{
    // Schedule the command to be executed
    switch(target_thread)
    {
        case THREAD_MAIN:
            // Schedule on the Arma's main thread (through the RVController)
            controller()->post_task(std::move(function));
            break;

        case THREAD_ISOLATED:
            // Schedule on the RVController's asynchronous executor
            controller()->post_task_async(std::move(function));
            break;
    }
}
