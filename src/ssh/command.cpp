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
#include <utility>

using namespace mafia;

ssh::Command::Command(std::string command_name, std::string command_description):
        _ssh_interface(std::move(command_name), std::move(command_description))
{
}

ssh::Command::~Command() = default;

std::string ssh::Command::execute(const std::string& command_name, int argc, char** argv)
{
    const auto parse_result = _ssh_interface.parse(argc, argv);
    return execute_command(command_name, parse_result);
}

void ssh::Command::init()
{
    init_ssh_interface(_ssh_interface.add_options());
}

std::string ssh::Command::help()
{
    return _ssh_interface.help();
}

ssh::ScheduledCommandHandler
ssh::Command::schedule(ssh::QueuedFunction_t&& function, ssh::QueuedFunctionThread target_thread)
{
    return ScheduledCommandHandler(
            new ScheduledCommand(std::move(function), target_thread),
            Command::_do_schedule
    );
}

void ssh::Command::_do_schedule(ssh::ScheduledCommand* command_ptr)
{
    // Schedule the command to be executed
    switch(command_ptr->_thread)
    {
        case THREAD_MAIN:
            // No need to actually execute on Arma's thread; lock a
            break;

        case THREAD_SSH_WORKER:
            break;

        case THREAD_ISOLATED:
            break;
    }

    // Once everything is done being executed, delete
    delete command_ptr;
};
