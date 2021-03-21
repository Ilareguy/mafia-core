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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_VAR_SPACE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_VAR_SPACE_H

#include "serialization.h"
#include "game_variable.h"
#include "game_value.h"

namespace mafia::game_types
{
    class GameVarSpace: public SerializeClass
    {
    public:
        map_string_to_class<GameVariable, auto_array<GameVariable>> variables;
        GameVarSpace* parent;
        bool dummy;

    public:
        GameVariable* get_variable(std::string_view varName);
        void set_variable(mafia::game_types::String varName, GameValue newValue);
        void delete_variable(std::string_view varName);
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_VAR_SPACE_H
