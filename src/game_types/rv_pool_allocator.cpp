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

#include "rv_pool_allocator.h"
#include "../mafia.h"
#include "../loader.h"
#include "../rv_controller.h"
#include "../memory_utility.h"

using namespace mafia::game_types;

void* RVPoolAllocator::allocate(size_t count)
{
    static auto allocatorBase = mafia::controller()->get_loader()->get_allocator();
    typedef void* (__thiscall* allocFunc)(RVPoolAllocator*, size_t /*count*/);
    auto alloc = reinterpret_cast<allocFunc>(allocatorBase->poolFuncAlloc);
    auto allocation = alloc(this, count);
    return allocation;
}

void RVPoolAllocator::deallocate(void* data)
{
    static auto allocatorBase = mafia::controller()->get_loader()->get_allocator();
    typedef void(__thiscall* deallocFunc)(RVPoolAllocator*, void* /*data*/);
    auto dealloc = reinterpret_cast<deallocFunc>(allocatorBase->poolFuncDealloc);
    dealloc(this, data);
}
