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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_NAMESPACE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_NAMESPACE_H

#include "../game_data.h"
#include "../v_table.h"
#include "../game_variable.h"
#include "../../containers/map_string_to_class.h"

namespace mafia::game_types::game_data
{
    class Namespace: public mafia::game_types::GameData, public DummyVtable
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

    public:
        Namespace() noexcept;
        size_t hash() const;

    public:
        mafia::containers::MapStringToClass<GameVariable, mafia::containers::AutoArray<GameVariable>> _variables {};
        mafia::game_types::String _name {""};
        bool _1 {false};
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_NAMESPACE_H
