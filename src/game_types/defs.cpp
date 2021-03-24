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

#include "defs.h"
#include <utility>

using namespace mafia::game_types;

constexpr RegisteredSQFFunction::RegisteredSQFFunction() noexcept = default;

RegisteredSQFFunction::RegisteredSQFFunction(std::shared_ptr<RegisteredSQFFunctionImpl> func_) noexcept:
        _function(std::move(func_)) {}

void RegisteredSQFFunction::clear() noexcept
{
    _function = nullptr;
}

bool RegisteredSQFFunction::has_function() const noexcept
{
    return _function.get() != nullptr;
}

void mafia::game_types::set_game_value_vtable(uintptr_t vtable)
{
    GameValue::__vptr_def = vtable;
}
