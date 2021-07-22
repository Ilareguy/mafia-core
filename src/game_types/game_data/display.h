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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_DISPLAY_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_DISPLAY_H

#include "../game_data.h"

namespace mafia::game_types::game_data
{
    class Display: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

    public:
        Display() noexcept;
        size_t hash() const;

    public:
        void* display {nullptr};
    };
}

namespace std
{
    template<>
    struct hash<mafia::game_types::game_data::Display>
    {
        size_t operator()(const mafia::game_types::game_data::Display& x) const
        {
            return x.hash();
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_DISPLAY_H
