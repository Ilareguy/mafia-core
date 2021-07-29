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

#ifndef DEF_MAFIA_CORE_RUNTIME_MAFIA_H
#define DEF_MAFIA_CORE_RUNTIME_MAFIA_H

#ifdef __GNUC__
#define CDECL __attribute__ ((__cdecl__))
#else
#undef CDECL
#define CDECL __cdecl
#endif

#ifdef __GNUC__
#define DLLEXPORT __attribute__((visibility("default")))
#else
#define DLLEXPORT __declspec(dllexport)
#endif

#ifdef MAFIA_CORE
#define MAFIA_API_FUNC
#else
#define MAFIA_API_FUNC DLLEXPORT
#endif

#include "mafia_errors.h"
#include <cstdint>
#include <string_view>
#include <fmt/format.h>

namespace mafia
{

    /**
     * API version that a DLL module was compiled against. Mafia Core DLL version at runtime may be different.
     */
    namespace api_version
    {
        constexpr uint8_t major = 0;
        constexpr uint8_t minor = 0;
        constexpr uint8_t revision = 1;
        constexpr const char* suffix = "-alpha";
    }

    namespace runtime::version
    {
        extern "C" {
        DLLEXPORT uint8_t major;
        DLLEXPORT uint8_t minor;
        DLLEXPORT uint8_t revision;
        DLLEXPORT char* suffix;
        }
    }

    namespace runtime::log
    {
        // Don't put declared here in ::mafia::log because they would clash with these declared in ``../logging.h``.
        extern "C" {
        DLLEXPORT void (CDECL* _log_info)(const char*) = nullptr;
        DLLEXPORT void (CDECL* _log_debug)(const char*) = nullptr;
        DLLEXPORT void (CDECL* _log_warning)(const char*) = nullptr;
        DLLEXPORT void (CDECL* _log_error)(const char*) = nullptr;
        DLLEXPORT void (CDECL* _log_critical)(const char*) = nullptr;
        DLLEXPORT void (CDECL* _log_trace)(const char*) = nullptr;
        DLLEXPORT void (CDECL* _log_flush)(void) = nullptr;
        }

        void flush()
        {
            if (_log_flush)
            {
                _log_flush();
            }
        }

        template<typename... Args>
        void info(std::string_view fmt, Args&& ... args)
        {
            if (_log_info)
            {
                _log_info(fmt::format(fmt, args...).c_str());
            }
        }

        template<typename... Args>
        void debug(std::string_view fmt, Args&& ... args)
        {
            if (_log_debug)
            {
                _log_debug(fmt::format(fmt, args...).c_str());
            }
        }

        template<typename... Args>
        void warn(std::string_view fmt, Args&& ... args)
        {
            if (_log_warning)
            {
                _log_warning(fmt::format(fmt, args...).c_str());
            }
        }

        template<typename... Args>
        void error(std::string_view fmt, Args&& ... args)
        {
            if (_log_error)
            {
                _log_error(fmt::format(fmt, args...).c_str());
            }
        }

        template<typename... Args>
        void critical(std::string_view fmt, Args&& ... args)
        {
            if (_log_critical)
            {
                _log_critical(fmt::format(fmt, args...).c_str());
            }
        }

        template<typename... Args>
        void trace(std::string_view fmt, Args&& ... args)
        {
            if (_log_trace)
            {
                _log_trace(fmt::format(fmt, args...).c_str());
            }
        }
    }
}

#endif // DEF_MAFIA_CORE_RUNTIME_MAFIA_H
