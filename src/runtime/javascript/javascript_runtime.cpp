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
    constexpr void* duk_user_ptr = nullptr;

    javascript_context = duk_create_heap(nullptr, nullptr, nullptr, duk_user_ptr, &JavascriptRuntime::duktape_error);
    auto* ctx = javascript_context;
    if (!javascript_context)
    {
        log::critical("Duktape initialization failed!");
        return; // @TODO Signal runtime initialization failure somehow (return bool instead of void)
    }

    duk_push_string(
            ctx,
            R"(
        function hello(){ return "VERY NICE!"; }
        hello();
)"
    );

    if (duk_peval(ctx) != 0)
    {
        log::error("eval failed: {}", duk_safe_to_string(ctx, -1));
    }
    else
    {
        log::info("Success! Result is {}", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx);

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

void javascript::JavascriptRuntime::shutdown()
{
    if (javascript_context)
    {
        duk_destroy_heap(javascript_context);
        javascript_context = nullptr;
    }
}

Result javascript::JavascriptRuntime::load_module(const Module& module_to_load)
{
    log::info(
            "Javascript runtime requested to load module \"{}\" in {}", module_to_load.name(),
            module_to_load.directory_path());
    return runtime::Result::success();
}

Result javascript::JavascriptRuntime::unload_module(const Module& module_to_unload)
{
    log::info(
            "Javascript runtime requested to unload module \"{}\".", module_to_unload.name());
    return runtime::Result::success();
}

/*bool javascript::load_module(const ModuleInfo& info, ErrorBase& err)
{
    log::info("Javascript runtime requested to load module {} at {}", info.name, info.path);
    return false;
}*/
