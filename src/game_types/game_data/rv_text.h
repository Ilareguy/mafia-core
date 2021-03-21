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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_DAME_DATA_RV_TEXT_H
#define DEF_MAFIA_CORE_GAME_TYPES_DAME_DATA_RV_TEXT_H

#include "../game_data.h"

namespace mafia::game_types::game_data
{
    class RVText: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

    public:
        RVText() noexcept;

        size_t hash() const { return _private::pairhash(type_def, rv_text); }

        void* rv_text {};
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_DAME_DATA_RV_TEXT_H
