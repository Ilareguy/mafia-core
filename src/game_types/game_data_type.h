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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_TYPE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_TYPE_H

#include "rv_string.h"

namespace mafia::game_types
{
    enum class GameDataType
    {
        SCALAR,
        BOOL,
        ARRAY,
        STRING,
        NOTHING,
        ANY,
        NAMESPACE,
        NaN,
        CODE,
        OBJECT,
        SIDE,
        GROUP,
        TEXT,
        SCRIPT,
        TARGET,
        CONFIG,
        DISPLAY,
        CONTROL,
        NetObject,
        SUBGROUP,
        TEAM_MEMBER,
        TASK,
        DIARY_RECORD,
        LOCATION,
        end
    };

    GameDataType from_string(const String& name);
    std::string_view to_string(const GameDataType t);

    namespace _private
    {
        void add_game_data_type(String name, GameDataType type);
    }
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_TYPE_H
