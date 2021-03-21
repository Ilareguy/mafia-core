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

#include "vm_context.h"

using namespace mafia::game_types;

auto VMContext::add_callstack_item(Ref<callstack_item> newItem)
{
    return callstack.emplace_back(newItem);
}

void VMContext::throw_script_exception(GameValue value)
{
    exception_state = true;
    exception_value = std::move(value);
}

bool VMContext::is_scheduled() const
{
    return scheduled;
}

bool VMContext::is_serialization_enabled() const
{
    return serialenabled;
}

void VMContext::disable_serialization()
{
    serialenabled = false;
}