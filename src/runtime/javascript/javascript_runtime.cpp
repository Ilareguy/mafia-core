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
#include "duktape/duktape.h"
#include <future>

using namespace mafia;
using namespace mafia::runtime;

// https://duktape.org/guide.html#gettingstarted.3

namespace mafia::runtime::javascript::_private
{
    duk_context* javascript_context {nullptr};

    void duktape_error(void* user_data, const char* msg)
    {
        log::error("Javascript error: {}", msg);
        log::flush();
    }
}

void javascript::initialize()
{
    //log::info("Hello, World!");
    constexpr void* duk_user_ptr = nullptr;

    _private::javascript_context = duk_create_heap(nullptr, nullptr, nullptr, duk_user_ptr, &_private::duktape_error);
    auto* ctx = _private::javascript_context;
    if (!_private::javascript_context)
    {
        log::critical("Duktape initialization failed!");
        return; // @TODO Signal runtime initialization failure somehow (return bool instead of void)
    }


    duk_push_string(ctx, "print('Hello world!'); 123;");
    if (duk_peval(ctx) != 0)
    {
        log::error("eval failed: {}", duk_safe_to_string(ctx, -1));
    }
    else
    {
        log::info("Success! Result is {}", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx);

    log::flush();
}

void javascript::shutdown()
{
    if (_private::javascript_context)
    {
        duk_destroy_heap(_private::javascript_context);
        _private::javascript_context = nullptr;
    }
}

bool javascript::load_module(const ModuleInfo& info, ErrorBase& err)
{
    return false;
}
