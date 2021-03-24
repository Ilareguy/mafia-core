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

#include "rv_controller.h"
#include "sqf_functions.h"
#include "invoker.h"
#include "loader.h"

using namespace mafia;

RVController::RVController() = default;

RVController::~RVController()
{
    // Not disabling this will result in a crash upon DLL unloading
    _sqf_functions->setDisabled();
}

void RVController::initialize(uintptr_t stack_base)
{
    _invoker = std::make_shared<Invoker>(*this);
    _loader = std::make_shared<Loader>();

    _loader->init(stack_base);
}

std::string RVController::rv_call(std::string_view command, Arguments& args)
{
    // Find a handler for the given command
    auto handler_it = _command_handlers.find(std::string {command});

    if (handler_it == _command_handlers.end())
    {
        // No such command registered
        return "<unknown command>";
    }

    // Execute command
    return handler_it->second(args);
}

bool RVController::add_rv_command_handler(
        std::string_view command,
        RVController::RVCommandHandler_t handler
)
{
    auto already_registered = _command_handlers.find(std::string {command});
    if (already_registered != _command_handlers.end())
    {
        // Handler already defined for this command
        return false;
    }

    _command_handlers.insert(
            std::make_pair<std::string, RVCommandHandler_t>(
                    std::string {command},
                    std::move(handler)));

    return true;
}

std::shared_ptr<Loader>& RVController::get_loader()
{
    return _loader;
}

std::shared_ptr<SQFFunctions>& RVController::get_sqf_functions()
{
    return _sqf_functions;
}

std::shared_ptr<Invoker>& RVController::get_invoker()
{
    return _invoker;
}

std::shared_ptr<MissionEvents>& RVController::get_mission_events()
{
    return _mission_events;
}
