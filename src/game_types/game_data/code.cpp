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

#include "code.h"

using namespace mafia::game_types::game_data;

uintptr_t Code::type_def{0};
uintptr_t Code::data_type_def{0};
mafia::game_types::RVPoolAllocator* Code::pool_alloc_base{nullptr};

Code::Code() noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

void* Code::operator new(std::size_t)
{
    return pool_alloc_base->allocate(1);
}

void Code::operator delete(void* ptr_, std::size_t)
{
    return pool_alloc_base->deallocate(ptr_);
}
