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

#include "task_executor.h"

/*mafia::TaskExecutor::TaskExecutor() = default;
mafia::TaskExecutor::~TaskExecutor() = default;

void mafia::TaskExecutor::post_task(Task_t&& new_task)
{
    tf::Taskflow new_tf;
    new_tf.emplace([](){});

    std::lock_guard<std::mutex> _l(_mutex);
    _posted_tasks.push(new_task);
}

void mafia::TaskExecutor::post_task(Task_t&& task, Task_t&& then, TaskExecutor& then_executor)
{
    //
}

void mafia::TaskExecutor::run_tasks()
{
    std::lock_guard<std::mutex> _l(_mutex);
    if (!_posted_tasks.empty())
    {
        auto t = _posted_tasks.front();
        _posted_tasks.pop();
        run_task(std::move(t));
    }
}*/
