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

#include "./string.h"

using namespace mafia::game_types::game_data;

uintptr_t String::type_def {0};
uintptr_t String::data_type_def {0};
mafia::game_types::RVPoolAllocator* String::pool_alloc_base;

String::String() noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

String::String(const std::string& str_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    raw_string = mafia::game_types::String(str_);
}

String::String(const mafia::game_types::String& str_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    raw_string = str_;
}

String::String(const String& copy_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    raw_string = copy_.raw_string;
}

String::String(String&& move_) noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    _refcount = move_._refcount;
    raw_string = move_.raw_string;
    move_.raw_string.clear();
}

String& String::operator=(const String& copy_)
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    raw_string = copy_.raw_string;
    return *this;
}

String& String::operator=(String&& move_) noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
    _refcount = move_._refcount;
    raw_string = move_.raw_string;
    move_.raw_string.clear();
    return *this;
}

String::~String() {}

void* String::operator new(std::size_t)
{
    return pool_alloc_base->allocate(1);
}

void String::operator delete(void* ptr_, std::size_t)
{
    return pool_alloc_base->deallocate(ptr_);
}
