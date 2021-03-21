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
 * File created by Anthony Ilareguy on 21/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_GAME_TYPES_RV_POOL_ALLOCATOR_H
#define DEF_MAFIA_CORE_GAME_TYPES_RV_POOL_ALLOCATOR_H

#include "rv_allocator.h"

namespace mafia::game_types
{
    class RVPoolAllocator
    {
#ifndef __GNUC__
        //It is required, but GCC doesn't care about unused members and ignores the attribute, and thus warns about a ignored attribute
        [[maybe_unused]]
#endif
#ifdef _WIN64
        char pad_0x0000[0x48];  //0x0000
#else
        char pad_0x0000[0x24];  //0x0000
#endif
    public:
        const char* _allocName;

        int _1;
        int _2;
        int _3;
        int _4;
        int allocated_count;

        void* allocate(size_t count);
        void deallocate(void* data);
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_RV_POOL_ALLOCATOR_H
