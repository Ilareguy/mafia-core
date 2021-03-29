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
 * File created by Anthony Ilareguy on 28/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "runtime.h"
#include "module.h"
#include "module_info.h"
#include "errors.h"
#include "../logging.h"
#include "../dll_handle.h"

using namespace mafia;
using namespace std::literals::string_view_literals;

Runtime::Runtime(std::string_view dll_path)
{
    _handle = std::make_unique<DLLHandle>(dll_path);

    // @TODO Exceptions
    _initialize_func = _handle->get<InitializeFunction_t>("initialize"sv, true);
    _shutdown_func = _handle->get<ShutdownFunction_t>("shutdown"sv, true);
    _load_module_func = _handle->get<LoadModuleFunction_t>("load_module"sv, true);

    // Hook logging functions
    *(_handle->get < void(CDECL**)(const char*)>("_log_info"sv, true)) = [](const char* c) { log::_info(c); };
    *(_handle->get < void(CDECL**)(const char*)>("_log_debug"sv, true)) = [](const char* c) { log::_debug(c); };
    *(_handle->get < void(CDECL**)(const char*)>("_log_warning"sv, true)) = [](const char* c) { log::_warn(c); };
    *(_handle->get < void(CDECL**)(const char*)>("_log_error"sv, true)) = [](const char* c) { log::_error(c); };
    *(_handle->get < void(CDECL**)(const char*)>("_log_critical"sv, true)) = [](const char* c) { log::_critical(c); };
    *(_handle->get < void(CDECL**)(const char*)>("_log_trace"sv, true)) = [](const char* c) { log::_trace(c); };
    *(_handle->get < void(CDECL**)(void)>("_log_flush"sv, true)) = []() { log::flush(); };
}

Runtime::~Runtime()
{
    _handle = nullptr;
}

void Runtime::initialize()
{
    if (_initialize_func)
    {
        _initialize_func();
    }
}

void Runtime::shutdown()
{
    if (_shutdown_func)
    {
        _shutdown_func();
    }
}

bool Runtime::load_module(const ModuleInfo& info, runtime::ErrorBase& err)
{
    return false;
}
