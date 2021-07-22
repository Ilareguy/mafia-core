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
 * File created by Anthony Ilareguy on 24/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_TASK_H
#define DEF_MAFIA_CORE_TASK_H


#include "non_copyable.h"
#include <cstdint>
#include <memory>
#include <functional>
#include <tuple>
#include <system_error>

namespace helper
{
    template<int... Is>
    struct index
    {
    };

    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...>
    {
    };

    template<int... Is>
    struct gen_seq<0, Is...> : index<Is...>
    {
    };
}

namespace mafia
{
    class TaskResult
    {
    public:
        TaskResult() : TaskResult(false) {}

        explicit TaskResult(bool successful) : _successful(successful),
                                               _error_message(nullptr),
                                               _error_code(0) {}

        TaskResult(const TaskResult& other) : TaskResult(other._successful,
                                                         other._error_message,
                                                         other._error_code) {}

        TaskResult(const TaskResult&& other) noexcept: _successful(other._successful),
                                                       _error_message(other._error_message),
                                                       _error_code(other._error_code) {}

        explicit TaskResult(const char* error) : TaskResult(error, 0) {}

        TaskResult(const char* error, uint32_t error_code) : _successful(false),
                                                             _error_message(error),
                                                             _error_code(error_code) {}

        TaskResult(bool successful, const char* error, uint32_t error_code) : _successful(successful),
                                                                              _error_message(error),
                                                                              _error_code(error_code) {}

        virtual ~TaskResult() = default;

    public:
        /**
         * @return True upon successful completion of the task; false otherwise.
         */
        [[nodiscard]] bool successful() const { return _successful; }

        [[nodiscard]] const char* error() const { return _error_message; }

        [[nodiscard]] uint32_t error_code() const { return _error_code; }

    private:
        const char* _error_message;
        const uint32_t _error_code;
        const bool _successful;
    };

    template<typename TaskReturn_t>
    class TaskResultTyped : TaskResult
    {
    public:
        explicit TaskResultTyped(TaskReturn_t return_value) : TaskResult(true), _result(std::move(return_value)) {}

        TaskResultTyped(const TaskResultTyped& other) : TaskResult(other), _result(other._result) {}

        TaskResultTyped(const TaskResultTyped&& other) noexcept: _result(std::move(other._result)),
                                                                 TaskResult(std::move(other)) {}

        TaskReturn_t result() const { return _result; }

    private:
        const TaskReturn_t _result;
    };

    /**
     * Task base class.
     */
    class Task : private NonCopyable
    {
    public:
        Task(Task&& other) noexcept: _run_result(std::move(other._run_result)),
                                     _was_run(other._was_run)
        {
            other._run_result = nullptr;
        }

        virtual ~Task() = default;

        [[nodiscard]] const TaskResult* raw_result() const { return _run_result.get(); }

        [[nodiscard]] bool was_run() const noexcept { return _was_run; }

        void run()
        {
            if (_was_run) return;
            try
            {
                _run_result = std::unique_ptr<TaskResult>(do_run());
                do_on_success();
            } catch (std::system_error& e)
            {
                _run_result = std::make_unique<TaskResult>(e.what(), e.code().value());
                do_on_error();
            } catch (std::exception& e)
            {
                _run_result = std::make_unique<TaskResult>(e.what(), 0);
                do_on_error();
            } catch (...)
            {
                _run_result = std::make_unique<TaskResult>("Unknown exception occurred", 0);
                do_on_error();
            }

            _was_run = true;
            do_on_completion();
        }

    protected:
        Task() = default;

        virtual TaskResult* do_run() = 0;
        virtual void do_on_success() = 0;
        virtual void do_on_error() = 0;
        virtual void do_on_completion() = 0;
        virtual bool callable() = 0;

    private:
        std::unique_ptr<TaskResult> _run_result {nullptr};
        bool _was_run {false};
    };

    template<typename Ret, typename... Args>
    class TaskTyped : public Task
    {
    private:
        typedef std::function<Ret(Args...)> Callable_t;
        typedef std::function<void(const char*, uint32_t)> Error_t;
        typedef std::function<void(Ret)> Success_t;
        typedef std::function<void(void)> Completion_t;
        typedef std::tuple<Args...> Args_t;

    public:
        template<typename F, typename... A>
        explicit TaskTyped(F&& f, A&& ... a) :
                _callable(std::forward<F>(f)),
                _args(std::forward<Args>(a)...) {}

        /*explicit TaskTyped<Ret, Args...>(TaskTyped<Ret, Args...>&& other) noexcept:
                _callable(std::move(other._callable)),
                _success(std::move(other._success)),
                _error(std::move(other._error)),
                _completion(std::move(other._completion)),
                _args(std::move(other._args))
        {
            other._callable = nullptr;
            other._success = nullptr;
            other._error = nullptr;
            other._completion = nullptr;
            other._args = nullptr;
        }*/

        ~TaskTyped() override = default;

        //operator Task*() { return (Task*) this; }

        bool callable() override
        {
            return !was_run() && _callable;
        }

        void on_success(Success_t&& s)
        {
            _success = std::forward<Success_t>(s);
        }

        void on_error(Error_t&& e)
        {
            _error = std::forward<Error_t>(e);
        }

        void on_completion(Completion_t&& c)
        {
            _completion = std::forward<Completion_t>(c);
        }

    protected:
        TaskResult* do_run() override
        {
            return (TaskResult*) new TaskResultTyped<Ret>(_run(_args));
        }

        void do_on_success() override
        {
            if (_success) _success(((TaskResultTyped<Ret>*) raw_result())->result());
        }

        void do_on_error() override
        {
            if (_error) _error(raw_result()->error(), raw_result()->error_code());
        }

        void do_on_completion() override
        {
            if (_completion) _completion();
        }

    private:
        template<typename... A, int... Is>
        Ret _run(std::tuple<A...>& tup, ::helper::index<Is...>)
        {
            return _callable(std::get<Is>(tup)...);
        }

        template<typename... A>
        Ret _run(std::tuple<A...>& tup)
        {
            return _run(tup, helper::gen_seq<sizeof...(A)> {});
        }

    private:
        Callable_t _callable;
        Success_t _success;
        Error_t _error;
        Completion_t _completion;
        Args_t _args;
    };

    template<typename F, typename... A, typename R = typename std::result_of<F(A...)>::type>
    std::unique_ptr<TaskTyped<R, A...>> make_task(F&& f, A&& ... a)
    {
        return std::make_unique<TaskTyped<R, A...>>(std::forward<F>(f), std::forward<A>(a)...);
    }
}

#endif //DEF_MAFIA_CORE_TASK_H
