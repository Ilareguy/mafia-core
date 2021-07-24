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
#include "../mafia_runtime.h"
#include "duktape/duktape.h"

namespace mafia::runtime::javascript
{
    extern "C" {
    MAFIA_API_FUNC ::mafia::RuntimeAPI* CDECL get_runtime();
    }

    class JavascriptRuntime: public ::mafia::RuntimeAPI
    {
    public:
        virtual ~JavascriptRuntime();
        void initialize() override;
        void shutdown() override;

    private:
        static void duktape_error(void* user_data, const char* msg);

    private:
        duk_context* javascript_context {nullptr};
    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_JAVASCRIPT_RUNTIME_H
