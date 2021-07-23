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


#ifndef DEF_MAFIA_CORE_SSH_COMMAND_H
#define DEF_MAFIA_CORE_SSH_COMMAND_H

#include "scheduled_command.h"
#include <string>
#include <cxxopts.hpp>

namespace mafia
{
    class SSHServer;
}

namespace mafia::ssh
{

    class Command
    {
    public:
        Command(std::string command_name, std::string command_description);
        virtual ~Command();
        void init();
        std::string execute(const std::string& command_name, int argc, char** argv, ::mafia::SSHServer&);

        /**
         * @return Returns the help string for a command
         */
        std::string help();

    protected:
        /**
         * Schedules a function to be run on the given target thread.
         * @param function The function to be invoked
         * @param target_thread Target thread. See ``enum QueuedFunctionThread``
         * @return Returns a unique_ptr to a ScheduledCommand object. Once this unique_ptr goes out of scope, the
         * function is actually scheduled for execution. This gives you time to configure the command before it is
         * executed.
         */
        static ScheduledCommandHandler schedule(QueuedFunction_t&& function, QueuedFunctionThread target_thread);

        /**
         * Your command should execute now with the given arguments. This will be invoked from a worker thread to
         * prevent hanging the program.
         * @param command_name Command name that was requested to run
         * @param args Parsed command arguments
         * @return Return a string message that will be sent over to the connected SSH client
         */
        virtual std::string execute_command(const std::string& command_name, const cxxopts::ParseResult& args, ::mafia::SSHServer&) = 0;

        /**
         * Implement this method to add options to your SSH interface.
         */
        virtual void init_ssh_interface(cxxopts::OptionAdder&&) = 0;

    private:
        static void _do_schedule(ScheduledCommand*);

    private:
        cxxopts::Options _ssh_interface;
    };
}

#endif // DEF_MAFIA_CORE_SSH_COMMAND_H
