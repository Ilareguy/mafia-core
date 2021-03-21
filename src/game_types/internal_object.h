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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_INTERNAL_OBJECT_H
#define DEF_MAFIA_CORE_GAME_TYPES_INTERNAL_OBJECT_H

#include "game_value.h"

namespace mafia::game_types
{
    class InternalObject: public GameValue
    {
    public:
        InternalObject();
        InternalObject(const GameValue& value_);
        InternalObject(const InternalObject& copy_);
        InternalObject(InternalObject&& move_) noexcept;
        InternalObject& operator=(InternalObject&& move_) noexcept;

        InternalObject& operator=(const InternalObject& copy_);

        bool operator<(const InternalObject& compare_) const;
        bool operator>(const InternalObject& compare_) const;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_INTERNAL_OBJECT_H
