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

#include <string>
#include <functional>
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
        typedef std::function<void()> ScheduledFunction_t;
        enum QueuedFunctionThread
        {
            /* Main Arma thread. Invoking tasks here will hang the game until the task returns.
             * It is safe to invoke SQF functions in here. Prefer this when you need to run quick tasks that need to
             * invoke SQF functions. */
            THREAD_MAIN = 0,

            /* A new isolated thread spawned for your task. Prefer this for long-running tasks or for tasks that do
             * not need to invoke SQF functions. */
            THREAD_ISOLATED
        };

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
        static void schedule(ScheduledFunction_t&& function, QueuedFunctionThread target_thread);

        /**
         * Your command should execute now with the given arguments. This will be invoked from a worker thread to
         * prevent hanging the program.
         * @param command_name Command name that was requested to run
         * @param args Parsed command arguments
         * @return Return a string message that will be sent over to the connected SSH client
         */
        virtual std::string
        execute_command(const std::string& command_name, const cxxopts::ParseResult& args, ::mafia::SSHServer&) = 0;

        /**
         * Implement this method to add options to your SSH interface.
         */
        virtual void init_ssh_interface(cxxopts::OptionAdder&&) = 0;

    private:
        cxxopts::Options _ssh_interface;
    };
}

#endif // DEF_MAFIA_CORE_SSH_COMMAND_H
