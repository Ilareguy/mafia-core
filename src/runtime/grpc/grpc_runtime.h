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


#ifndef DEF_MAFIA_CORE_GRPC_RUNTIME_H
#define DEF_MAFIA_CORE_GRPC_RUNTIME_H

#include "../mafia.h"
#include "../mafia_runtime.h"

namespace mafia::runtime::grpc
{
#ifndef MAFIA_CORE
    extern "C" {
    MAFIA_API_FUNC ::mafia::RuntimeAPI* CDECL get_runtime();
    }
#endif

    class GrpcRuntime: public ::mafia::RuntimeAPI
    {
    public:
        virtual ~GrpcRuntime();
        void initialize() override;
        void shutdown() override;
        Result load_module(const Module& module_to_load) override;
        Result unload_module(const Module& module_to_unload) override;
        char* eval(const char* code) override;

    private:
        //
    };
}

#endif //DEF_MAFIA_CORE_GRPC_RUNTIME_H
