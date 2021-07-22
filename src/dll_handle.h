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
 * File created by Anthony Ilareguy on 28/02/2021.
 * [File Description]
 *
 ********************************************************
 *
 * [Credits]
 *
 ********************************************************/
#ifndef DEF_MAFIA_CORE_DLL_HANDLE_H
#define DEF_MAFIA_CORE_DLL_HANDLE_H

#include "non_copyable.h"
#include <string_view>

namespace mafia
{
    class DLLHandle: public NonCopyable
    {
    public:
        class DLLOpenError: public std::exception
        {
        public:
            DLLOpenError(const char* const message) : std::exception(message){}
        };

        class SymbolNotFound: public std::exception
        {
        public:
            SymbolNotFound(const char* const message) : std::exception(message){}
        };

    public:
        /**
         * Throws `DLLOpenError` if DLL couldn't be loaded.
         */
        explicit DLLHandle(std::string_view path);

        ~DLLHandle();

        /**
         * Throws `SymbolNotFound` if the symbol couldn't be found.
         */
        template<typename Symbol_t>
        Symbol_t get(std::string_view symbol, bool mandatory = false);

    private:
        void* _raw_handle;
    };
}

#include "dll_handle.tcc"

#endif // DEF_MAFIA_CORE_DLL_HANDLE_H
