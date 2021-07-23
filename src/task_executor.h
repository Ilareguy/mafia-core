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
 * File created by Anthony Ilareguy on 2021-07-23.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_TASK_EXECUTOR_H
#define DEF_MAFIA_CORE_TASK_EXECUTOR_H

#include <functional>

namespace mafia
{
    /**
     * Classes that extend TaskExecutor are capable of accepting tasks that need to be executed either on their own
     * managed thread(s) or asynchronously on a generic worker thread.
     */
    class TaskExecutor
    {
    public:
        typedef std::function<void()> Task_t;

    public:
        /**
         * Registers the given task to be run on the TaskExecutor. Steals the given task.
         */
        virtual void post_task(Task_t&&) = 0;

        /**
         * Registers the given task to be run on the TaskExecutor. Steals the given task. Once the task has completed,
         * then ``then`` task will be posted back on the given ``then_executor``.
         * @param task
         * @param then
         * @param then_executor
         */
        virtual void post_task(Task_t&& task, Task_t&& then, TaskExecutor& then_executor) = 0;

    protected:
        /**
         * Runs tasks that were previously posted.
         */
        virtual void run_tasks() = 0;

    };
}

#endif // DEF_MAFIA_CORE_TASK_EXECUTOR_H
