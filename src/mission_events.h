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

#include "game_types/game_value.h"
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

        /**
         * Registers an event handler to be executed when a specific event occurs.
         * @param name
         * @param handler
         * @return
         */
        bool add_event_handler(std::string_view name, EventHandler_t handler);

        /**
         *
         * @param event_name_
         * @param params_
         * @return
         */
        bool rv_event(const std::string& event_name_, const game_types::GameValue& params_);

    public:
        static game_types::GameValue client_event_handler(game_types::GameValue& left_arg,
                                                             game_types::GameValue& right_arg);
        static void pre_init(game_types::GameValue& args_);
        static void pre_pre_init(game_types::GameValue& args_);
        static void pre_start(game_types::GameValue& args_);
        static void post_start(game_types::GameValue& args_);
        static void post_init(game_types::GameValue& args_);
        static void mission_ended(game_types::GameValue& args_);

    private:
        static bool _stopping;

    private:
        std::unordered_map<std::string, EventHandler_t> _event_handlers;
    };
}


#endif //DEF_MAFIA_CORE_MISSION_EVENTS_H
