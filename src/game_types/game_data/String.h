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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_STRING_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_STRING_H

#include "../game_data.h"

namespace mafia::game_types
{
    class RVPoolAllocator;
}

namespace mafia::game_types::game_data
{
    class String: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;

    public:
        String() noexcept;
        String(const std::string& str_);
        String(const mafia::game_types::String& str_);
        String(const String& copy_);
        String(String&& move_) noexcept;
        String& operator=(const String& copy_);
        String& operator=(String&& move_) noexcept;
        ~String();
        size_t hash() const;
        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        //protected:
        //    static thread_local game_data_pool<game_data_string> _data_pool;

    public:
        mafia::game_types::String raw_string {""};
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_STRING_H
