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

#include "mission_events.h"
#include "mafia.h"
#include "logging.h"
#include "invoker.h"
#include "rv_controller.h"

using namespace std::literals::string_view_literals;

mafia::MissionEvents::MissionEvents()
{
    //add_event_handler("pre_start"sv, [](const game_value& params_){});
}

mafia::MissionEvents::~MissionEvents() = default;

bool mafia::MissionEvents::add_event_handler(std::string_view name, mafia::MissionEvents::EventHandler_t handler)
{
    return false;
}

bool mafia::MissionEvents::rv_event(const std::string& event_name_, const mafia::game_types::GameValue& params_)
{
    mafia::log::info("{}: \"{}\"(size:{})", __FUNCTION__, event_name_, params_.size());

    auto handler = _event_handlers.find(event_name_);
    if (handler != _event_handlers.end())
    {
        bool all = false;
        // If we are stopping a mission it is assumed that threads will be
        // stopped and joined here. Deadlocks can occur if we do not open up
        // the invoker to all threads.
        if (event_name_ == "mission_ended"sv)
        {
            all = true;
        }

        Invoker::_invoker_unlock eh_lock(*(mafia::controller()->get_invoker().get()), all);
        //game_value params = invoke_raw_nolock(_get_variable_func, &_mission_namespace, &var_name);
        handler->second(params_);
        return true;
    }
    return false;
}

mafia::game_types::GameValue mafia::MissionEvents::client_event_handler(
        mafia::game_types::GameValue& left_arg,
        mafia::game_types::GameValue& right_arg
)
{
    return mafia::game_types::GameValue();
}

void mafia::MissionEvents::pre_init(mafia::game_types::GameValue& args_)
{

}

void mafia::MissionEvents::pre_pre_init(mafia::game_types::GameValue& args_)
{

}

void mafia::MissionEvents::pre_start(mafia::game_types::GameValue& args_)
{

}

void mafia::MissionEvents::post_start(mafia::game_types::GameValue& args_)
{

}

void mafia::MissionEvents::post_init(mafia::game_types::GameValue& args_)
{

}

void mafia::MissionEvents::mission_ended(mafia::game_types::GameValue& args_)
{

}
