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
 * File created by Anthony Ilareguy on 22/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_LOGGING_H
#define DEF_MAFIA_CORE_LOGGING_H

#include <fmt/format.h>

namespace mafia::log
{
    void _info(std::string_view);
    void _debug(std::string_view);
    void _warn(std::string_view);
    void _error(std::string_view);
    void _critical(std::string_view);
    void _trace(std::string_view);
    void flush();

    template<typename... Args>
    void info(std::string_view fmt, Args&& ... args)
    {
        _info(fmt::format(fmt, args...));
    }

    template<typename... Args>
    void debug(std::string_view fmt, Args&& ... args)
    {
        _debug(fmt::format(fmt, args...));
    }

    template<typename... Args>
    void warn(std::string_view fmt, Args&& ... args)
    {
        _warn(fmt::format(fmt, args...));
    }

    template<typename... Args>
    void error(std::string_view fmt, Args&& ... args)
    {
        _error(fmt::format(fmt, args...));
    }

    template<typename... Args>
    void critical(std::string_view fmt, Args&& ... args)
    {
        _critical(fmt::format(fmt, args...));
    }

    template<typename... Args>
    void trace(std::string_view fmt, Args&& ... args)
    {
        _trace(fmt::format(fmt, args...));
    }
}

#endif // DEF_MAFIA_CORE_LOGGING_H
