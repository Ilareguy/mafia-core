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
 * File created by Anthony Ilareguy on 2021-07-27.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_RUNTIME_DUKTAPE_GLOBALS_H
#define DEF_MAFIA_CORE_RUNTIME_DUKTAPE_GLOBALS_H

#include "duktape/duktape.h"
#include "../mafia.h"
#include <fmt/format.h>

namespace mafia::runtime::javascript
{
    /**
     * Duktape stack: negative idx starts on top of stack; positive starts at bottom.
     * i.e.: bottom [ some_object, some_object, some_object, some_object, some_object ] top (items pushed to the right)
     *                 ^0 or -5^    ^1 or -4^    ^2 or -3^    ^3 or -2^    ^4 or -1^
     *
     * Duktape C functions return values:
     * C function returns 1  = Javascript function returns value on top of the stack;
     * C function returns 0  = Javascript functions returns undefined/nothing;
     * C function returns <0 = Javascript throws error (use DUK_RET_xxx constants).
     */

    namespace detail
    {
        duk_ret_t log(duk_context* ctx, void(* log_func)(const char*))
        {
            // Stack entry: [ message ]

            // Disallow new()
            if (duk_is_constructor_call(ctx))
            {
                return DUK_RET_TYPE_ERROR;
            }

            // Fetch message to log
            const char* message = duk_safe_to_string(ctx, -1);
            duk_pop(ctx); // stack: []

            // @TODO: There should be a way of knowing which module invoked the logging function

            // Log
            log_func(message);

            return 0;
        }

        /*duk_ret_t mafia_global(duk_context* ctx)
        {
            // Stack entry: []

            if (duk_is_constructor_call(ctx))
            {
                return DUK_RET_TYPE_ERROR;
            }

            duk_push_this(ctx);

            return 0;
        }*/
    }

    void register_globals(duk_context* ctx)
    {
        // see https://wiki.duktape.org/howtonativeconstructor

        { // Global: ``Mafia``
            duk_push_global_object(ctx);
            duk_push_string(ctx, "Mafia");
            auto object_idx = duk_push_object(ctx);  /* -> [ global, "Mafia", object {} ] */

            // Mafia.api_version = "..."
            duk_push_string(ctx, "api_version");
            duk_push_string(
                    ctx, fmt::format(
                            "{}.{}.{}{}",
                            ::mafia::api_version::major,
                            ::mafia::api_version::minor,
                            ::mafia::api_version::revision,
                            ::mafia::api_version::suffix
                    ).c_str());
            duk_def_prop(
                    ctx, object_idx,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE);
            // -> [ global, "Mafia", object { api_version: "..." } ]

            // Mafia.runtime_version = "..."
            duk_push_string(ctx, "runtime_version");
            duk_push_string(
                    ctx, fmt::format(
                            "{}.{}.{}{}",
                            ::mafia::runtime::version::major,
                            ::mafia::runtime::version::minor,
                            ::mafia::runtime::version::revision,
                            ::mafia::runtime::version::suffix
                    ).c_str());
            duk_def_prop(
                    ctx, object_idx,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE);
            // -> [ global, "Mafia", object { api_version: "...", "runtime_version": "..." } ]

            // Finalize Mafia object
            duk_def_prop(
                    ctx, 0,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);
            // -> [ global ]
            duk_pop(ctx);
            // -> []
        }

        { // Global ``Log``
            duk_push_global_object(ctx);
            duk_push_string(ctx, "Log");
            auto object_idx = duk_push_object(ctx);
            // -> [ global, "Log", object {} ]

            duk_push_string(ctx, "info");
            duk_push_c_function(ctx, [](auto* ctx) { return detail::log(ctx, log::_log_info); }, 1);
            duk_def_prop(
                    ctx, object_idx,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE);

            duk_push_string(ctx, "warning");
            duk_push_c_function(ctx, [](auto* ctx) { return detail::log(ctx, log::_log_warning); }, 1);
            duk_def_prop(
                    ctx, object_idx,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE);

            duk_push_string(ctx, "error");
            duk_push_c_function(ctx, [](auto* ctx) { return detail::log(ctx, log::_log_error); }, 1);
            duk_def_prop(
                    ctx, object_idx,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE);

            duk_push_string(ctx, "debug");
            duk_push_c_function(ctx, [](auto* ctx) { return detail::log(ctx, log::_log_debug); }, 1);
            duk_def_prop(
                    ctx, object_idx,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE);

            // Finalize Log object
            duk_def_prop(
                    ctx, 0,
                    DUK_DEFPROP_HAVE_VALUE |
                    DUK_DEFPROP_HAVE_WRITABLE |
                    DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);
            duk_pop(ctx);
            // -> []
        }
    }
}

#endif // DEF_MAFIA_CORE_RUNTIME_DUKTAPE_GLOBALS_H
