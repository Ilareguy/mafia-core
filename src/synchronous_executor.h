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


#ifndef DEF_MAFIA_CORE_SYNCHRONOUS_EXECUTOR_H
#define DEF_MAFIA_CORE_SYNCHRONOUS_EXECUTOR_H

#include "task_executor.h"
#include <mutex>
#include <queue>
#include <taskflow/taskflow.hpp>

namespace mafia
{
    /**
     * A TaskExecutor that runs tasks in a blocking fashion when ``run_tasks()`` is invoked.
     */
    class SynchronousTaskExecutor: public TaskExecutor
    {
    public:
        SynchronousTaskExecutor();
        virtual ~SynchronousTaskExecutor();

        void post_task(Task_t&&) override;
        void post_task(Task_t&& task, Task_t&& then, TaskExecutor& then_executor) override;

    protected:
        void run_tasks() override;

    private:
        std::mutex _m;
        std::queue<std::unique_ptr<tf::Taskflow>> _tasks;
        tf::Executor _executor;
    };
}

#endif // DEF_MAFIA_CORE_SYNCHRONOUS_EXECUTOR_H
