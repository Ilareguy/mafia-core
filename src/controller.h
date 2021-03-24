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
 * File created by Anthony Ilareguy on 23/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_CONTROLLER_H
#define DEF_MAFIA_CORE_CONTROLLER_H

#include "non_copyable.h"
#include <memory>

namespace mafia
{
    class Task;
    class Controller;
    class TaskDispatcherBase;

    /**
     * Controller base class.
     */
    class Controller : public NonCopyable
    {
    public:
        Controller();
        virtual ~Controller();

    protected:
        virtual void do_initialize() = 0;

    private:
        void _initialize();

    private:
        bool _initialized {false};
    };

    /**
     * Base class for a Controller with a TaskDispatcher.
     * @tparam JobDispatcherImpl The TaskDispatcher class responsible for taking tasks and execute them or schedule
     * them for execution.
     */
    template<typename JobDispatcherImpl>
    class ControllerTyped : public Controller
    {
    public:

        /**
         * Sends a task to the job dispatcher, to be executed at some point, possibly asynchronously. The moment and
         * thread where the given task will be executed depends on the JobDispatcherImpl.
         * @param task The task to execute. The Controller takes ownership of the task.
         */
        template<typename Task_t>
        void post(std::unique_ptr<Task_t> t)
        {
            _job_dispatcher->post(std::move(t));
            //t = nullptr;
        }

    protected:
        void do_initialize() override
        {
            _job_dispatcher = std::make_unique<JobDispatcherImpl>(this);
        }

    private:
        std::unique_ptr<JobDispatcherImpl> _job_dispatcher {nullptr};
    };

    /**
     * Base class for a job dispatcher implementation, which takes in tasks from a Controller.
     */
    class TaskDispatcherBase : public NonCopyable
    {
    public:
        explicit TaskDispatcherBase(Controller*);
        virtual ~TaskDispatcherBase();

        virtual void post(std::unique_ptr<Task> task) = 0;

    protected:
        Controller* const _controller;
    };
}

#endif //DEF_MAFIA_CORE_CONTROLLER_H
