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

#include "config.h"
#include "../../pair_hash.h"

using namespace mafia::game_types::game_data;

uintptr_t Config::type_def {0};
uintptr_t Config::data_type_def {0};

Config::Config() noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

size_t Config::hash() const
{
    return mafia::pair_hash(type_def, config);
}
