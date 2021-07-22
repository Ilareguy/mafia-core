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

#include "game_data_type.h"
#include <map>

using namespace mafia::game_types;
using namespace std::literals::string_view_literals;

namespace mafia::game_types::_private
{
    static std::map<std::string, GameDataType> additional_types;
}

void mafia::game_types::_private::add_game_data_type(const mafia::game_types::String name_, const GameDataType type_)
{
    additional_types[static_cast<std::string>(name_)] = type_;
}

GameDataType mafia::game_types::from_string(const String& name)
{
    //I know this is ugly. Feel free to make it better
    if (name == "SCALAR"sv)
    { return GameDataType::SCALAR; }
    if (name == "BOOL"sv)
    { return GameDataType::BOOL; }
    if (name == "ARRAY"sv)
    { return GameDataType::ARRAY; }
    if (name == "STRING"sv)
    { return GameDataType::STRING; }
    if (name == "NOTHING"sv)
    { return GameDataType::NOTHING; }
    if (name == "ANY"sv)
    { return GameDataType::ANY; }
    if (name == "NAMESPACE"sv)
    { return GameDataType::NAMESPACE; }
    if (name == "NaN"sv)
    { return GameDataType::NaN; }
    if (name == "CODE"sv)
    { return GameDataType::CODE; }
    if (name == "OBJECT"sv)
    { return GameDataType::OBJECT; }
    if (name == "SIDE"sv)
    { return GameDataType::SIDE; }
    if (name == "GROUP"sv)
    { return GameDataType::GROUP; }
    if (name == "TEXT"sv)
    { return GameDataType::TEXT; }
    if (name == "SCRIPT"sv)
    { return GameDataType::SCRIPT; }
    if (name == "TARGET"sv)
    { return GameDataType::TARGET; }
    if (name == "CONFIG"sv)
    { return GameDataType::CONFIG; }
    if (name == "DISPLAY"sv)
    { return GameDataType::DISPLAY; }
    if (name == "CONTROL"sv)
    { return GameDataType::CONTROL; }
    if (name == "NetObject"sv)
    { return GameDataType::NetObject; }
    if (name == "SUBGROUP"sv)
    { return GameDataType::SUBGROUP; }
    if (name == "TEAM_MEMBER"sv)
    { return GameDataType::TEAM_MEMBER; }
    if (name == "TASK"sv)
    { return GameDataType::TASK; }
    if (name == "DIARY_RECORD"sv)
    { return GameDataType::DIARY_RECORD; }
    if (name == "LOCATION"sv)
    { return GameDataType::LOCATION; }
    auto found = _private::additional_types.find(static_cast<std::string>(name));
    if (found != _private::additional_types.end())
    {
        return found->second;
    }
    return GameDataType::end;
}

std::string_view mafia::game_types::to_string(const GameDataType t)
{
    switch (t)
    {
        case GameDataType::SCALAR: return "SCALAR"sv;
        case GameDataType::BOOL: return "BOOL"sv;
        case GameDataType::ARRAY: return "ARRAY"sv;
        case GameDataType::STRING: return "STRING"sv;
        case GameDataType::NOTHING: return "NOTHING"sv;
        case GameDataType::ANY: return "ANY"sv;
        case GameDataType::NAMESPACE: return "NAMESPACE"sv;
        case GameDataType::NaN: return "NaN"sv;
        case GameDataType::CODE: return "CODE"sv;
        case GameDataType::OBJECT: return "OBJECT"sv;
        case GameDataType::SIDE: return "SIDE"sv;
        case GameDataType::GROUP: return "GROUP"sv;
        case GameDataType::TEXT: return "TEXT"sv;
        case GameDataType::SCRIPT: return "SCRIPT"sv;
        case GameDataType::TARGET: return "TARGET"sv;
        case GameDataType::CONFIG: return "CONFIG"sv;
        case GameDataType::DISPLAY: return "DISPLAY"sv;
        case GameDataType::CONTROL: return "CONTROL"sv;
        case GameDataType::SUBGROUP: return "SUBGROUP"sv;
        case GameDataType::TEAM_MEMBER:return "TEAM_MEMBER"sv;
        case GameDataType::TASK: return "TASK"sv;
        case GameDataType::DIARY_RECORD: return "DIARY_RECORD"sv;
        case GameDataType::LOCATION: return "LOCATION"sv;
        default:;
    }
    for (auto& it : _private::additional_types)
    {
        if (it.second == t)
        {
            return it.first;
        }
    }
    return "";
}
