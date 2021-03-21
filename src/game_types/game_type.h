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

namespace mafia::game_types
{
    enum class GameType
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
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_TYPE_H
