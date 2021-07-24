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

#include "mafia_runtime.h"
#include "../shared.h"
#include "../dll_handle.h"
#include <memory>

namespace mafia
{
    class DLLHandle;
    class ModuleInfo;
    class RuntimeAPI;
    class Runtime
    {
    public:
        typedef RuntimeAPI* (CDECL* GetRuntimeFunction_t)(void);

    public:
        explicit Runtime(const char* dll_path);
        ~Runtime();

        /**
         * @return Returns direct access to the RuntimeAPI implemented in the DLL.
         */
        inline RuntimeAPI& api(){ return *_runtime_api; }

    private:
        DLLHandle _dll_handle;
        GetRuntimeFunction_t _get_runtime_function;
        RuntimeAPI* _runtime_api;

    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_H
