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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_ARRAY_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_ARRAY_H

#include "../game_data.h"

namespace mafia::game_types::game_data
{
    class Array: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;

    public:
        Array();
        explicit Array(size_t size_);
        explicit Array(const std::vector<GameValue>& init_);
        Array(const std::initializer_list<GameValue>& init_);
        explicit Array(auto_array<GameValue>&& init_);
        Array(const Array& copy_);
        Array(Array&& move_) noexcept;
        Array& operator=(const Array& copy_);
        Array& operator=(Array&& move_) noexcept;
        ~Array();
        auto_array<GameValue> data;

        auto length() const { return data.count(); }

        size_t hash() const { return _private::pairhash(type_def, data.hash()); }

        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_ARRAY_H
