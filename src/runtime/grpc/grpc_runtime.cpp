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
 * Created by Anthony Ilareguy on 2023-03-18.
 * [File Description]
 *
 ********************************************************
 *
 * [Credits]
 *
 ********************************************************/

#include "grpc_runtime.h"
#include "../mafia_module.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace mafia;
using namespace mafia::runtime;

::mafia::RuntimeAPI* grpc::get_runtime()
{
    return new GrpcRuntime();
}

grpc::GrpcRuntime::~GrpcRuntime() = default;

void grpc::GrpcRuntime::initialize()
{
    log::flush();
}

void grpc::GrpcRuntime::shutdown()
{
}

Result grpc::GrpcRuntime::load_module(const Module& module_to_load)
{
    /*namespace fs = boost::filesystem;
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
    duk_call(ctx, 0);
    log::info("Compiled main.js evaluates to: {}", duk_get_string(ctx, -1));
    */
    return runtime::Result::success();
}

Result grpc::GrpcRuntime::unload_module(const Module& module_to_unload)
{
    log::info(
            "gRPC runtime requested to unload module \"{}\".", module_to_unload.name());
    return runtime::Result::success();
}

char* grpc::GrpcRuntime::eval(const char* code) { return nullptr; }
