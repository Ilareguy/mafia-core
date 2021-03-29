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

#ifndef DEF_MAFIA_CORE_RUNTIME_H
#define DEF_MAFIA_CORE_RUNTIME_H

#include "../shared.h"
#include <string_view>
#include <memory>

namespace mafia
{
    namespace runtime
    {
        struct ErrorBase;
    }

    class DLLHandle;
    class ModuleInfo;
    class Runtime
    {
    public:
        Runtime(std::string_view dll_path);
        ~Runtime();
        void initialize();
        void shutdown();
        bool load_module(const ModuleInfo& info, runtime::ErrorBase& err);

    public:
        typedef void (CDECL *InitializeFunction_t)(void);
        typedef void (CDECL *ShutdownFunction_t)(void);
        typedef bool (CDECL* LoadModuleFunction_t)(const ModuleInfo& info, runtime::ErrorBase& err);

    private:
        std::unique_ptr<DLLHandle> _handle {nullptr};
        InitializeFunction_t _initialize_func = nullptr;
        ShutdownFunction_t _shutdown_func = nullptr;
        LoadModuleFunction_t _load_module_func = nullptr;
    };
}

#endif //DEF_MAFIA_CORE_RUNTIME_H
