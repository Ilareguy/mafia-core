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

#include "game_var_space.h"

using namespace mafia::game_types;

GameVariable* GameVarSpace::get_variable(std::string_view varName)
{
    auto& var = variables.get(varName.data());
    if (!variables.is_null(var))
    {
        return &var;
    }
    if (parent)
    {
        return parent->get_variable(varName);
    }
    return nullptr;
}

void GameVarSpace::set_variable(mafia::game_types::String varName, GameValue newValue)
{
    auto& var = variables.get(varName.data());
    if (!variables.is_null(var))
    {
        var.value = newValue;
    }
    variables.insert(GameVariable(varName, newValue));
}

void GameVarSpace::delete_variable(std::string_view varName)
{
    auto& var = variables.get(varName.data());
    if (!variables.remove(varName) && parent)
    {
        parent->delete_variable(varName);
    }
}