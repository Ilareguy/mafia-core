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

#ifndef DEF_MAFIA_CORE_RUNTIME_JAVASCRIPT_RUNTIME_H
#define DEF_MAFIA_CORE_RUNTIME_JAVASCRIPT_RUNTIME_H

#include "../mafia.h"

namespace mafia::runtime::javascript
{
    extern "C" {
    MAFIA_API_FUNC void CDECL initialize();
    MAFIA_API_FUNC void CDECL shutdown();

    /**
     * Your runtime should load the module specified and return true on success and false on error.
     * If an error occurred, you must set `err` to a relevant error object describing what happened.
     */
    MAFIA_API_FUNC bool CDECL load_module(const runtime::ModuleInfo& info, runtime::ErrorBase& err);
    }
}

#endif // DEF_MAFIA_CORE_RUNTIME_JAVASCRIPT_RUNTIME_H
