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

#include "javascript_runtime.h"
#include "../mafia_module.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace mafia;
using namespace mafia::runtime;

// https://duktape.org/guide.html#gettingstarted.3

::mafia::RuntimeAPI* javascript::get_runtime()
{
    return new JavascriptRuntime();
}

javascript::JavascriptRuntime::~JavascriptRuntime() = default;


void javascript::JavascriptRuntime::duktape_error(void* user_data, const char* msg)
{
    log::error("Javascript error: {}", msg);
    log::flush();
}

void javascript::JavascriptRuntime::initialize()
{
    auto duk_user_ptr = (void*) this;

    ctx = duk_create_heap(nullptr, nullptr, nullptr, duk_user_ptr, &JavascriptRuntime::duktape_error);
    if (!ctx)
    {
        log::critical("Duktape initialization failed!");
        return; // @TODO Signal runtime initialization failure somehow (return bool instead of void)
    }

    init_globals();

    /**
     * @TODO:
     * - Expose logging functions to Javascript;
     * - Expose RV functions to Javascript;
     * - Allow reloading Javascript runtime DLL at runtime;
     * - Write initial Javascript/Typescript interface files, which will be available to third-party modules written
     *      in Javascript/Typescript;
     * - Allow loading, unloading and reloading Javascript modules at runtime;
     * - Allow sending commands to Mafia from a console or something to control the server;
     */

    log::flush();
}

void javascript::JavascriptRuntime::init_globals()
{
    // see https://wiki.duktape.org/howtonativeconstructor

    // Global: ``Mafia``
    {
        duk_push_c_function(
                ctx,
                [](duk_context* ctx) -> duk_ret_t {
                    if (duk_is_constructor_call(ctx))
                    {
                        return DUK_RET_TYPE_ERROR;
                    }

                    duk_push_this(ctx);

                    return 0;
                }, 0
        );

        /* Push Mafia.prototype object. */
        duk_push_object(ctx);  /* -> stack: [ Mafia proto ] */

        /* Set Mafia.prototype.version. */
        duk_push_c_function(
                ctx, [](duk_context*) -> duk_ret_t {
                    return 0;
                }, 0 /*nargs*/);
        duk_put_prop_string(ctx, -2, "version");

        /* Set Mafia.prototype = proto */
        duk_put_prop_string(ctx, -2, "prototype");  /* -> stack: [ Mafia ] */

        /* Finally, register Mafia to the global object */
        duk_put_global_string(ctx, "Mafia");  /* -> stack: [ ] */
    }
}

void javascript::JavascriptRuntime::shutdown()
{
    if (ctx)
    {
        duk_destroy_heap(ctx);
        ctx = nullptr;
    }
}

Result javascript::JavascriptRuntime::load_module(const Module& module_to_load)
{
    namespace fs = boost::filesystem;
    constexpr auto main_js_file = "main.js";
    const fs::path path {module_to_load.directory_path()};

    // Check that main.js is present
    if (!fs::is_regular_file(path / main_js_file))
    {
        return runtime::Result::error(fmt::format("Missing {}", main_js_file).c_str());
    }

    // Load & compile main.js
    {
        std::ifstream in_file {fs::path {path / main_js_file}.string()};
        std::stringstream in_buf;
        in_buf << in_file.rdbuf();
        duk_push_string(ctx, in_buf.str().c_str());
        duk_push_string(ctx, main_js_file);
        if (duk_pcompile(ctx, DUK_COMPILE_FUNCTION) != 0)
        {
            const auto error_message = duk_safe_to_string(ctx, -1);
            return runtime::Result::error(error_message);
        }
    }

    // Execute
    // duk_call(ctx, 0);
    // log::info("Compiled main.js evaluates to: {}", duk_get_string(ctx, -1));

    return runtime::Result::success();
}

Result javascript::JavascriptRuntime::unload_module(const Module& module_to_unload)
{
    log::info(
            "Javascript runtime requested to unload module \"{}\".", module_to_unload.name());
    return runtime::Result::success();
}

char* javascript::JavascriptRuntime::eval(const char* code)
{
    std::string res {};
    duk_push_string(ctx, code);
    auto eval_result = duk_peval(ctx);

    if (eval_result != 0)
    {
        res = fmt::format("Javascript error: {}", duk_safe_to_string(ctx, -1));
    }
    else
    {
        res = duk_safe_to_string(ctx, -1);
    }
    duk_pop(ctx);

    auto len = res.length() + 1;
    char* res_c = new char[res.length() + 1];
    strncpy_s(res_c, len, res.c_str(), len);
    return res_c;
}
