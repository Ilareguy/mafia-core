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
#include "mission_events.h"
#include "invoker.h"
#include "loader.h"
#include "ssh_server.h"
#include "runtime.h"
#include "logging.h"

using namespace mafia;
using namespace std::literals::string_view_literals;

RVController::RVController(): _async_executor(3) {}

RVController::~RVController() = default;

void RVController::initialize(uintptr_t stack_base)
{
    _invoker = std::make_shared<Invoker>(*this);
    _loader = std::make_shared<Loader>();
    _sqf_functions = std::make_shared<SQFFunctions>();
    _mission_events = std::make_shared<MissionEvents>();
    _loader->init(stack_base);
    _mission_events->initialize();

    // @TODO Load config file

    _ssh_server = std::make_unique<SSHServer>("test"sv, "test"sv, 23456);

    _javascript_runtime = std::make_unique<Runtime>(
            R"(@mafia\mafia-runtime-javascript.dll)"
    );
    _javascript_runtime->api().initialize();
}

void RVController::shutdown()
{
    if (_javascript_runtime)
    {
        _javascript_runtime->api().shutdown();
        _javascript_runtime = nullptr;
    }

    _mission_events->shutdown();

    _mission_events = nullptr;
    _invoker = nullptr;
    _sqf_functions = nullptr;
    _loader = nullptr;
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

void RVController::on_game_frame()
{
    run_tasks();
}

bool RVController::rv_signal(
        const std::string& extension_name,
        const std::string& signal_name,
        const game_types::GameValue& args
)
{
    // @TODO
    /*auto signal_module = extensions::get().modules().find(extension_name);
    if (signal_module == extensions::get().modules().end()) {
        return false;
    }
    auto signal_func_it = signal_module->second.signal_funcs.find(signal_name);
    module::on_signal_func signal_func;
    if (signal_func_it == signal_module->second.signal_funcs.end()) {

        signal_func = reinterpret_cast<module::on_signal_func>(GetProcAddress(signal_module->second.dll_handle, signal_name.c_str())); // #TODO why?! The signal module function thingy is commented out.. also has a #TODO with ?! on it

        if (!signal_func)
            return false;
        else
            signal_module->second.signal_funcs[signal_name] = signal_func;
    } else {
        signal_func = signal_func_it->second;
    }
    _invoker_unlock signal_lock(this);
    signal_func(args);*/
    return true;
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

std::shared_ptr<Loader> RVController::get_loader()
{
    return _loader;
}

std::shared_ptr<SQFFunctions> RVController::get_sqf_functions()
{
    return _sqf_functions;
}

std::shared_ptr<Invoker> RVController::get_invoker()
{
    return _invoker;
}

std::shared_ptr<MissionEvents> RVController::get_mission_events()
{
    return _mission_events;
}

void RVController::post_task_async(TaskExecutor::Task_t&& t)
{
    _async_executor.post_task(std::move(t));
}

void
RVController::post_task_async(TaskExecutor::Task_t&& task, TaskExecutor::Task_t&& then, TaskExecutor& then_executor)
{
    _async_executor.post_task(std::move(task), std::move(then), then_executor);
}

void RVController::try_load_module(
        std::string_view name,
        RVController::ModuleLoadCompleteFunction_t&& then_
)
{
    try
    {
        auto new_module = Module::try_load(std::string {name}, (*_javascript_runtime));
        _javascript_runtime->loaded_modules.push_back(new_module);
        _javascript_runtime->post_task(
                [then = std::forward<RVController::ModuleLoadCompleteFunction_t>(then_), new_module]() {
                    then(new_module);
                }
        );
    }
    catch (const std::exception& e)
    {
        log::warn("Could not load module in directory `{}`: {}", name, e.what());
        throw e;
    }
}

void RVController::try_unload_module(
        std::string_view name,
        RVController::ModuleUnloadCompleteFunction_t&& then
)
{

}

RuntimeAPI& RVController::javascript_runtime()
{
    return _javascript_runtime->api();
}
