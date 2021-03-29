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
#include <utility>

using namespace std::literals::string_view_literals;

mafia::MissionEvents::MissionEvents()
{
    //add_event_handler("pre_start"sv, [](const game_value& params_){});
}

mafia::MissionEvents::~MissionEvents() = default;

void mafia::MissionEvents::initialize()
{
    add_event_handler("pre_init"sv, [this](const game_types::GameValue& a) { pre_init(a); });
    add_event_handler("pre_pre_init"sv, [this](const game_types::GameValue& a) { pre_pre_init(a); });
    add_event_handler("pre_start"sv, [this](const game_types::GameValue& a) { pre_start(a); });
    add_event_handler("post_start"sv, [this](const game_types::GameValue& a) { post_start(a); });
    add_event_handler("post_init"sv, [this](const game_types::GameValue& a) { post_init(a); });
    add_event_handler("mission_ended"sv, [this](const game_types::GameValue& a) { mission_ended(a); });
}

void mafia::MissionEvents::shutdown()
{
    _event_handlers.clear();
}

bool mafia::MissionEvents::add_event_handler(std::string_view name_, mafia::MissionEvents::EventHandler_t handler)
{
    std::string name(name_);
    if (_event_handlers.find(name) != _event_handlers.end())
    {
        // @TODO: Exceptions
        return false;
    }
    _event_handlers[name] = std::move(handler);

    return true;
}

bool mafia::MissionEvents::rv_event(const std::string& event_name_, const mafia::game_types::GameValue& params_)
{
    //log::debug("{}: \"{}\"(size:{})", __FUNCTION__, event_name_, params_.size());

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
        const mafia::game_types::GameValue& left_arg,
        const mafia::game_types::GameValue& right_arg
)
{
    return mafia::game_types::GameValue();
}

void mafia::MissionEvents::pre_init(const game_types::GameValue& args_)
{
    log::debug("{} with args {}.", __FUNCTION__, static_cast<std::string>(args_));
}

void mafia::MissionEvents::pre_pre_init(const game_types::GameValue& args_)
{
    log::debug("{} with args {}.", __FUNCTION__, static_cast<std::string>(args_));
}

void mafia::MissionEvents::pre_start(const game_types::GameValue& args_)
{
    log::debug("{} with args {}.", __FUNCTION__, static_cast<std::string>(args_));
    _event_handler_function = mafia::controller()->get_sqf_functions()->register_sqf_function(
            "MafiaClientEvent"sv,
            "Forwarder used to call functions in Intercept Plugins"sv,
            userFunctionWrapper<client_event_handler>,
            game_types::GameDataType::ANY,
            game_types::GameDataType::ARRAY,
            game_types::GameDataType::ARRAY
    ); // @TODO what is this? Currently unused.
}

void mafia::MissionEvents::post_start(const game_types::GameValue& args_)
{
    log::debug("{} with args {}.", __FUNCTION__, static_cast<std::string>(args_));
}

void mafia::MissionEvents::post_init(const game_types::GameValue& args_)
{
    log::debug("{} with args {}.", __FUNCTION__, static_cast<std::string>(args_));
}

void mafia::MissionEvents::mission_ended(const game_types::GameValue& args_)
{
    log::debug("{} with args {}.", __FUNCTION__, static_cast<std::string>(args_));
}
