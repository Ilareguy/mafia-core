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
#ifndef MAFIA_CORE
    extern "C" {
    MAFIA_API_FUNC ::mafia::RuntimeAPI* CDECL get_runtime();
    }
#endif

    class JavascriptRuntime: public ::mafia::RuntimeAPI
    {
    public:
        virtual ~JavascriptRuntime();
        void initialize() override;
        void shutdown() override;
        Result load_module(const Module& module_to_load) override;
        Result unload_module(const Module& module_to_unload) override;
        char* eval(const char* code) override;

    private:
        static void duktape_error(void* user_data, const char* msg);
        void init_globals();

    private:
        duk_context* ctx {nullptr};
    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_JAVASCRIPT_RUNTIME_H
