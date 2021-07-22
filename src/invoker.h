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

#ifndef DEF_MAFIA_CORE_INVOKER_H
#define DEF_MAFIA_CORE_INVOKER_H

#include "sqf_functions.h"
#include "non_copyable.h"
#include "game_types/game_data.h"
#include "game_types/defs.h"
#include "game_types/game_state.h"
#include <mutex>
#include <string>
#include <unordered_map>

namespace mafia
{
    class Arguments;
    class RVController;
    class MissionEvents;

    class Invoker
    {
    public:
        explicit Invoker(RVController& rv_controller);
        ~Invoker();

        /*!
        @name Invoke Functions
        */
        //!@{
        /*!@{
        @brief Nular invoke functions.

        Invoke a nular SQF function, either by function pointer or by name.

        @return Returns a raw types::rv_game_value struct. This is often
        passed into a types::game_value to be unwound into the appropriate
        return type, or passed to a complex argument constructor such as `object`
        or `group`, etc.
        */
        static game_types::GameValue invoke_raw_nolock(const game_types::NularFunction function_) noexcept;
        game_types::GameValue invoke_raw(std::string_view function_name_) const;
        //!@}

        /*!@{
        @brief Unary invoke functions.

        Invoke a unary SQF function, either by function pointer or by name and
        argument signatures.

        @return Returns a raw types::rv_game_value struct. This is often
        passed into a types::game_value to be unwound into the appropriate
        return type, or passed to a complex argument constructor such as `object`
        or `group`, etc.
        */
        static game_types::GameValue invoke_raw_nolock(
                const game_types::UnaryFunction function_,
                const game_types::GameValue& right_
        ) noexcept;
        game_types::GameValue invoke_raw(
                std::string_view function_name_,
                const game_types::GameValue& right_
        ) const;
        game_types::GameValue invoke_raw(
                std::string_view function_name_,
                const game_types::GameValue& right_,
                const std::string& right_type_
        ) const;
        //!@}

        /*!@{
        @brief Binary invoke functions.

        Invoke a binary SQF function, either by function pointer or by name and
        argument signatures.

        @return Returns a raw types::rv_game_value struct. This is often
        passed into a types::game_value to be unwound into the appropriate
        return type, or passed to a complex argument constructor such as `object`
        or `group`, etc.
        */
        static game_types::GameValue
        invoke_raw_nolock(
                const game_types::BinaryFunction function_,
                const game_types::GameValue& left_,
                const game_types::GameValue& right_
        ) noexcept;

        game_types::GameValue invoke_raw(
                std::string_view function_name_,
                const game_types::GameValue& left_,
                const game_types::GameValue& right_
        ) const;

        game_types::GameValue invoke_raw(
                std::string_view function_name_,
                const game_types::GameValue& left_,
                const std::string& left_type_,
                const game_types::GameValue& right_,
                const std::string& right_type_
        ) const;
        //!@}
        //!@}

        /*!
        @brief Opens the RV Engine up for a period of time to allow client threads
        access.

        This is easily the most important function in the invoker as it is the
        function that dictates when other threads can access the game engine.

        This function is invoked from a per-frame handler in SQF, each frame this
        controller function is called and the Invoker will unlock all client threads
        for 3ms of access to the RV Engine and then it will close. Before it closes
        though it invokes the clients `on_frame()` function, which is a blocking
        function call that each client plugin can define for guaranteed per-frame
        execution.
        */
        bool do_invoke_period();

    protected:
        static game_types::GameValue _mafia_event(
                game_types::GameValueParameter left_arg_,
                game_types::GameValueParameter right_arg_
        );
        static game_types::GameValue _mafia_signal(
                game_types::GameValueParameter left_arg_,
                game_types::GameValueParameter right_arg_
        );
        static game_types::GameValue _mafia_do_invoke_period();
        static game_types::GameValue _mafia_registerTypes(const game_types::GameValue& left_arg_);

    private:
        std::string _init_invoker(const Arguments& args_);
        std::string _test_invoker(const Arguments& args_);
        std::string _invoker_begin_register(const Arguments& args_);
        std::string _invoker_register(const Arguments& args_);
        std::string _invoker_end_register(const Arguments& args_);

    public:
        static game_types::GameState* sqf_game_state;
        static bool invoker_accessible;
        static bool invoker_accessible_all;

        std::unordered_map<uintptr_t, std::string_view> type_map;
        std::unordered_map<std::string_view, std::pair<uintptr_t, uintptr_t>> type_structures;

    private:
        std::recursive_mutex _invoke_mutex;
        std::mutex _state_mutex;
        std::condition_variable _invoke_condition;
        std::atomic<uint32_t> _thread_count;
        std::atomic<uint32_t> _thread_waiting_for_lock_count;
        uintptr_t _sqf_game_state;
        std::string _registration_type;
        RegisteredSQFFunction _event_function;
        RegisteredSQFFunction _signal_function;
        RegisteredSQFFunction _registerTypes_function;
        RegisteredSQFFunction _do_invoke_period_function;
        RVController& _rv_controller;

    protected:
        /*!
        @brief A RAII style mutex handler for access to the RV Engine

        Provides a safe way to unlock other threads or invokes to the RV engine.
        Will unlock an invoker passed in, by default only allowing access to invokes
        from the invokers own thread. If `all_threads_` is set to true in the constructor
        it will unlock all threads attempting to invoke. It is also possible to
        delay unlocking, so that the unlock can happen based on a condition but
        still retain the safety of being bound to the scope in which it was declared.
        */
        class _invoker_unlock: public NonCopyable
        {
        public:
            explicit _invoker_unlock(Invoker& instance_, bool all_threads_ = false, bool delayed_ = false);
            ~_invoker_unlock();
            void unlock();
        protected:
            bool _unlocked;
            Invoker& _instance;
            bool _all;
        };

        friend class MissionEvents;
    };
}

#endif //DEF_MAFIA_CORE_INVOKER_H
