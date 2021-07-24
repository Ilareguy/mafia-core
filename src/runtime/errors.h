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

#ifndef DEF_MAFIA_CORE_RUNTIME_ERRORS_H
#define DEF_MAFIA_CORE_RUNTIME_ERRORS_H

#include <exception>

namespace mafia::runtime
{
    struct ErrorBase: public std::exception
    {
        explicit ErrorBase(char const* const m) noexcept: std::exception(m){}
    };

    struct LoadModuleError: public ErrorBase
    {
        explicit LoadModuleError(char const* const m) noexcept: ErrorBase(m){}
    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_ERRORS_H
