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

    javascript_context = duk_create_heap(nullptr, nullptr, nullptr, duk_user_ptr, &JavascriptRuntime::duktape_error);
    if (!javascript_context)
    {
        log::critical("Duktape initialization failed!");
        return; // @TODO Signal runtime initialization failure somehow (return bool instead of void)
    }

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
        duk_push_string(javascript_context, in_buf.str().c_str());
        duk_push_string(javascript_context, main_js_file);
        if (duk_pcompile(javascript_context, DUK_COMPILE_FUNCTION) != 0)
        {
            const auto error_message = std::string{duk_safe_to_string(javascript_context, -1)};
            return runtime::Result::error(fmt::format("Javascript compilation failed: {}",
                                                      error_message).c_str());
        }
        // ? duk_pop(javascript_context);
    }

    // Execute
    duk_call(javascript_context, 0);
    log::info("Compiled main.js evaluates to: {}", duk_get_string(javascript_context, -1));

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
