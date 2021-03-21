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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_BOOL_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_BOOL_H

#include "../game_data.h"

namespace mafia::game_types::game_data
{
    class Bool: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;
        Bool();
        explicit Bool(bool val_);
        Bool(const Bool& copy_);
        Bool(Bool&& move_) noexcept;
        Bool& operator=(const Bool& copy_);
        Bool& operator=(Bool&& move_) noexcept;
        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        bool val;

        size_t hash() const { return _private::pairhash(type_def, val); }
        //protected:
        //    static thread_local game_data_pool<game_data_bool> _data_pool;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_BOOL_H
