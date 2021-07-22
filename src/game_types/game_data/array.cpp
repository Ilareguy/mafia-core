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

#include "array.h"
#include "../../pair_hash.h"
#include "../rv_pool_allocator.h"

using namespace mafia::game_types::game_data;

uintptr_t Array::type_def {0};
uintptr_t Array::data_type_def {0};
mafia::game_types::RVPoolAllocator* Array::pool_alloc_base {nullptr};

Array::Array()
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

Array::Array(size_t size_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    data.resize(size_);
}

Array::Array(const std::initializer_list<mafia::game_types::GameValue>& init_): data(init_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

Array::Array(mafia::containers::AutoArray<mafia::game_types::GameValue>&& init_): data(std::move(init_))
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

Array::Array(const std::vector<mafia::game_types::GameValue>& init_): data(init_.begin(), init_.end())
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

Array::Array(const Array& copy_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    data = copy_.data;
}

Array::Array(Array&& move_) noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    data = std::move(move_.data);
}

Array& Array::operator=(const Array& copy_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    data = copy_.data;
    return *this;
}

Array& Array::operator=(Array&& move_) noexcept
{
    if (this == &move_)
    {
        return *this;
    }
    data = std::move(move_.data);
    return *this;
}

Array::~Array() = default;

void* Array::operator new(std::size_t)
{
    return pool_alloc_base->allocate(1);
}

void Array::operator delete(void* ptr_, std::size_t)
{
    return pool_alloc_base->deallocate(ptr_);
}

auto Array::length() const
{
    return data.count();
}

size_t Array::hash() const
{
    return mafia::pair_hash(type_def, data.hash());
}
