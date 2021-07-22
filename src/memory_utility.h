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
 * File created by Anthony Ilareguy on 20/03/2021.
 * Functions related to searching and finding addresses in process memory for values, function pointers, variables, etc.
 *
 ********************************************************
 * 
 * This file is derived from the Intercept project.
 * See https://github.com/intercept/intercept
 * See https://github.com/intercept/intercept/blob/master/src/host/loader/loader.cpp
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_MEMORY_UTILITY_H
#define DEF_MAFIA_CORE_MEMORY_UTILITY_H

#include <string_view>

namespace mafia::memory_utility
{
    uintptr_t find_in_memory(uintptr_t baseAddress, uintptr_t moduleSize, const char* pattern, size_t patternLength);
    uintptr_t find_in_memory_pattern(
            uintptr_t baseAddress,
            uintptr_t moduleSize,
            const char* pattern,
            const char* mask,
            uintptr_t offset = 0
    );
    const char* get_RTTI_name(uintptr_t vtable);

    // Basically Windows' IsBadReadPtr(). See https://stackoverflow.com/questions/496034/most-efficient-replacement-for-isbadreadptr
    bool ptr_check(void* p);
}

#endif // DEF_MAFIA_CORE_MEMORY_UTILITY_H
