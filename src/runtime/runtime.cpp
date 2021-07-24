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
#include "errors.h"
#include "../logging.h"

using namespace mafia;
using namespace std::literals::string_view_literals;

Runtime::Runtime(const char* const dll_path): _dll_handle(dll_path)
{
    _get_runtime_function = _dll_handle.get<GetRuntimeFunction_t>("get_runtime"sv, true);
    _runtime_api = _get_runtime_function();

    // Hook logging functions
    *(_dll_handle.get < void(CDECL**)(const char*)>("_log_info"sv, true)) = [](const char* c) { log::_info(c); };
    *(_dll_handle.get < void(CDECL**)(const char*)>("_log_debug"sv, true)) = [](const char* c) { log::_debug(c); };
    *(_dll_handle.get < void(CDECL**)(const char*)>("_log_warning"sv, true)) = [](const char* c) { log::_warn(c); };
    *(_dll_handle.get < void(CDECL**)(const char*)>("_log_error"sv, true)) = [](const char* c) { log::_error(c); };
    *(_dll_handle.get < void(CDECL**)(const char*)>("_log_critical"sv, true))
    = [](const char* c) { log::_critical(c); };
    *(_dll_handle.get < void(CDECL**)(const char*)>("_log_trace"sv, true)) = [](const char* c) { log::_trace(c); };
    *(_dll_handle.get < void(CDECL**)(void) > ("_log_flush"sv, true)) = []() { log::flush(); };
}

Runtime::~Runtime()
{
    delete _runtime_api;
}
