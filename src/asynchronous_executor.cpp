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

#include "asynchronous_executor.h"

mafia::AsynchronousTaskExecutor::AsynchronousTaskExecutor(unsigned short int worker_thread_count):
        _executor(worker_thread_count) {}

mafia::AsynchronousTaskExecutor::~AsynchronousTaskExecutor()
{
    _executor.wait_for_all();
}

void mafia::AsynchronousTaskExecutor::post_task(mafia::TaskExecutor::Task_t&& t)
{
    auto new_taskflow = new tf::Taskflow();
    new_taskflow->emplace(t);
    std::lock_guard _l{_m};
    _tasks.push(new_taskflow);
}

void mafia::AsynchronousTaskExecutor::post_task(
        mafia::TaskExecutor::Task_t&& t,
        mafia::TaskExecutor::Task_t&& then,
        mafia::TaskExecutor& then_executor
)
{
    auto new_taskflow = new tf::Taskflow();
    auto task = new_taskflow->emplace(t);
    auto then_task = new_taskflow->emplace(
            [&]() {
                then_executor.post_task(std::move(then));
            }
    );
    task.precede(then_task);

    std::lock_guard _l{_m};
    _tasks.push(new_taskflow);
}

void mafia::AsynchronousTaskExecutor::run_tasks()
{
    std::lock_guard _l{_m};
    if (!_tasks.empty())
    {
        auto t = _tasks.front();
        _tasks.pop();
        _executor.run(*t);
    }
}
