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

#include "game_variable.h"
#include "game_value_impl.h"
#include "../containers/compact_array.h"

using namespace mafia::game_types;

GameVariable::GameVariable() = default;

GameVariable::GameVariable(mafia::game_types::String name_, GameValue&& val_, bool read_only_):
        name(std::move(name_)), value(std::move(val_)), read_only(read_only_) {}

GameVariable::GameVariable(mafia::game_types::String name_, const GameValue& val_, bool read_only_):
        name(std::move(name_)), value(val_), read_only(read_only_) {}

std::string_view GameVariable::get_map_key() const { return name; }

void GameVariable::get_name(char* buffer, int len) const
{
    std::copy(
            name.begin(), std::min(name.begin() + static_cast<size_t>(len), name.end()),
            mafia::containers::CompactArray<char>::iterator(buffer));
    buffer[len - 1] = 0;
}

void* GameVariable::get_val() const
{
    return value.impl->data;
}
