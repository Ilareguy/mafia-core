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
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "memory_utility.h"
#include <future>
#include <Windows.h>

uintptr_t mafia::memory_utility::find_in_memory(
        uintptr_t baseAddress,
        uintptr_t moduleSize,
        const char* pattern,
        size_t patternLength
)
{
    for (uintptr_t i = 0; i < moduleSize - patternLength; i++)
    {
        bool found = true;
        for (uintptr_t j = 0; j < patternLength; j++)
        {
            found &= pattern[j] == *reinterpret_cast<char*>(baseAddress + i + j);
            if (!found)
            {
                break;
            }
        }

        if (found)
        {
            return baseAddress + i;
        }
    }
    return 0;
}

uintptr_t mafia::memory_utility::find_in_memory_pattern(
        uintptr_t baseAddress, uintptr_t moduleSize, const char* pattern, const char* mask, uintptr_t offset
)
{
    const auto patternLength = static_cast<uintptr_t>(strlen(mask));

    for (uintptr_t i = 0; i < moduleSize - patternLength; i++)
    {
        bool found = true;
        for (uintptr_t j = 0; j < patternLength; j++)
        {
            found &= mask[j] == '?' || pattern[j] == *reinterpret_cast<char*>(baseAddress + i + j);
            if (!found)
            {
                break;
            }
        }

        if (found)
        {
            return baseAddress + i + offset;
        }
    }
    return static_cast<uintptr_t>(0x0u);
}

const char* mafia::memory_utility::get_RTTI_name(uintptr_t vtable)
{
    class v1
    {
        virtual void doStuff() {}
    };
    class v2: public v1
    {
        virtual void doStuff() {}
    };
    v2* v = (v2*) vtable;
    auto& typex = typeid(*v);
#ifdef __GNUC__
    auto test = typex.name();
#else
    auto test = typex.raw_name();
#endif
    return test;
}

bool mafia::memory_utility::ptr_check(void* p)
{
    MEMORY_BASIC_INFORMATION mbi = {nullptr};
    if (::VirtualQuery(p, &mbi, sizeof(mbi)))
    {
        DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                      PAGE_EXECUTE_WRITECOPY);
        bool b = !(mbi.Protect & mask);
        // check the page is not a guard page
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
        { b = true; }

        return b;
    }
    return true;
}
