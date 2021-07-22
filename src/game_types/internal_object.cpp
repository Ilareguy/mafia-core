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

#include "internal_object.h"
#include "game_data/object.h"
#include "game_data.h"

using namespace mafia::game_types;

InternalObject::InternalObject(): GameValue() {}

//InternalObject::InternalObject(const rv_GameValue &value_) : GameValue(value_) {}
InternalObject::InternalObject(const GameValue& value_): GameValue(value_)
{
    set_vtable(GameValue::__vptr_def);
}

InternalObject::InternalObject(const InternalObject& copy_): GameValue(copy_)
{
    set_vtable(GameValue::__vptr_def);
}

InternalObject::InternalObject(InternalObject&& move_) noexcept: GameValue(move_) { set_vtable(GameValue::__vptr_def); }

InternalObject& InternalObject::operator=(InternalObject&& move_) noexcept
{
    if (this == &move_)
    {
        return *this;
    }
    GameValue::operator=(std::move(move_));
    return *this;
}

InternalObject& InternalObject::operator=(const InternalObject& copy_)
{
    GameValue::operator=(copy_);
    return *this;
}

bool InternalObject::operator<(const InternalObject& compare_) const
{
    return static_cast<game_data::Object*>(data.get())->object <
           static_cast<game_data::Object*>(compare_.data.get())->object;
}

bool InternalObject::operator>(const InternalObject& compare_) const
{
    return static_cast<game_data::Object*>(data.get())->object >
           static_cast<game_data::Object*>(compare_.data.get())->object;
}
