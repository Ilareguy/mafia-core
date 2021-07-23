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

#ifndef DEF_MAFIA_CORE_SSH_SCHEDULED_COMMAND_H
#define DEF_MAFIA_CORE_SSH_SCHEDULED_COMMAND_H

#include <functional>

namespace mafia::ssh
{
    class ScheduledCommand;
    class Command;

    typedef std::unique_ptr<ScheduledCommand, std::function<void(ScheduledCommand*)>> ScheduledCommandHandler;
    typedef std::function<void(void)> QueuedFunction_t;
    typedef std::function<void(void)> QueuedFunctionThen_t;
    enum QueuedFunctionThread
    {
        /* Main Arma thread. Invoking tasks here will hang the game until the task return.
         * It is safe to invoke SQF functions in here. Prefer this when you need to run quick tasks that need to
         * invoke SQF functions. */
        THREAD_MAIN = 0,

        /* A new isolated thread spawned for your task. Prefer this for long-running tasks or for tasks that do
         * not need to invoke SQF functions. */
        THREAD_ISOLATED
    };

    class ScheduledCommand
    {
    public:
        explicit ScheduledCommand(QueuedFunction_t&&, QueuedFunctionThread);
        ~ScheduledCommand();

        /**
         * Registers a function to be invoked back on the SSH thread when your function has completed.
         */
        void then(QueuedFunctionThen_t&& f);

    private:
        QueuedFunctionThread _thread;
        QueuedFunction_t _function;
        QueuedFunctionThen_t _then {nullptr};

    private:
        friend class Command;
    };
}

#endif // DEF_MAFIA_CORE_SSH_SCHEDULED_COMMAND_H
