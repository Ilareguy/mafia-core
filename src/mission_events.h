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
 * File created by Anthony Ilareguy on 25/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_MISSION_EVENTS_H
#define DEF_MAFIA_CORE_MISSION_EVENTS_H

#include "game_types/game_data.h"
#include "sqf_functions.h"
#include <string_view>
#include <functional>

namespace mafia
{
    class MissionEvents
    {
    public:
        typedef std::function<void(const game_types::GameValue&)> EventHandler_t;

    public:
        MissionEvents();
        ~MissionEvents();
        void initialize();
        void shutdown();
        bool add_event_handler(std::string_view name, EventHandler_t handler);
        bool rv_event(const std::string& event_name_, const game_types::GameValue& params_);

    private:
        void pre_init(const game_types::GameValue& args_);
        void pre_pre_init(const game_types::GameValue& args_);
        void pre_start(const game_types::GameValue& args_);
        void post_start(const game_types::GameValue& args_);
        void post_init(const game_types::GameValue& args_);
        void mission_ended(const game_types::GameValue& args_);

    private:
        static game_types::GameValue client_event_handler(
                const game_types::GameValue& left_arg,
                const game_types::GameValue& right_arg
        );

    private:
        static bool _stopping;

    private:
        std::unordered_map<std::string, EventHandler_t> _event_handlers;
        RegisteredSQFFunction _event_handler_function;

    };
}


#endif //DEF_MAFIA_CORE_MISSION_EVENTS_H
