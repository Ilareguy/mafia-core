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
#include "logging.h"
#include "mafia.h"

using namespace mafia;
using namespace std::literals::string_view_literals;

Runtime::Runtime(const char* const dll_path): dll_handle(dll_path)
{
    get_runtime_function = dll_handle.get<GetRuntimeFunction_t>("get_runtime"sv, true);
    runtime_api = get_runtime_function();

    // Hook Mafia Core DLL runtime version
    *(dll_handle.get<uint8_t*>("major"sv, true)) = ::mafia::version::major;
    *(dll_handle.get<uint8_t*>("minor"sv, true)) = ::mafia::version::minor;
    *(dll_handle.get<uint8_t*>("revision"sv, true)) = ::mafia::version::revision;
    *(dll_handle.get<const char**>("suffix"sv, true)) = ::mafia::version::suffix;

    // Hook logging functions
    *(dll_handle.get < void(CDECL**)(const char*)>("_log_info"sv, true)) = [](const char* c) { log::_info(c); };
    *(dll_handle.get < void(CDECL**)(const char*)>("_log_debug"sv, true)) = [](const char* c) { log::_debug(c); };
    *(dll_handle.get < void(CDECL**)(const char*)>("_log_warning"sv, true)) = [](const char* c) { log::_warn(c); };
    *(dll_handle.get < void(CDECL**)(const char*)>("_log_error"sv, true)) = [](const char* c) { log::_error(c); };
    *(dll_handle.get < void(CDECL**)(const char*)>("_log_critical"sv, true))
    = [](const char* c) { log::_critical(c); };
    *(dll_handle.get < void(CDECL**)(const char*)>("_log_trace"sv, true)) = [](const char* c) { log::_trace(c); };
    *(dll_handle.get < void(CDECL**)(void) > ("_log_flush"sv, true)) = []() { log::flush(); };
}

Runtime::~Runtime()
{
    delete runtime_api;
}
