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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_IMPL_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_IMPL_H

// Sadly necessary to avoid circular dependency between game_data.h and game_value.h.

#include "../ref.h"
#include "game_data.h"

namespace mafia::game_types
{
    class GameValueImpl
    {
    public:
        GameValueImpl() = default;
        mafia::Ref<GameData> data;

    public:
        template<class T>
        mafia::Ref<T> get_as()
        {
            static_assert(
                    std::is_convertible_v<T, GameData>, "GameValue::get_as() can only convert to GameData types"
            );
            return static_cast<mafia::Ref<T>>(data);
        }

        template<class T>
        const mafia::Ref<T> get_as() const
        {
            static_assert(
                    std::is_convertible_v<T, GameData>, "GameValue::get_as() can only convert to GameData types"
            );
            return static_cast<const mafia::Ref<T>>(data);
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_IMPL_H
