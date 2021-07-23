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


#ifndef DEF_MAFIA_CORE_ASYNCHRONOUS_EXECUTOR_H
#define DEF_MAFIA_CORE_ASYNCHRONOUS_EXECUTOR_H

#include "task_executor.h"
#include <mutex>
#include <queue>
#include <taskflow/taskflow.hpp>

namespace mafia
{
    /**
     * A TaskExecutor that runs tasks asynchronously on worker threads when ``run_tasks()`` is invoked.
     */
    class AsynchronousTaskExecutor: public TaskExecutor
    {
    public:
        explicit AsynchronousTaskExecutor(unsigned short int worker_thread_count = 2);
        virtual ~AsynchronousTaskExecutor();

        void post_task(Task_t&&) override;
        void post_task(Task_t&& task, Task_t&& then, TaskExecutor& then_executor) override;
        void run_tasks() override;

    private:
        std::recursive_mutex _m;
        std::queue<tf::Taskflow*> _tasks;
        tf::Executor _executor;
    };
}

#endif // DEF_MAFIA_CORE_ASYNCHRONOUS_EXECUTOR_H
