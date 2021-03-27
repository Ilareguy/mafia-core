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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_VARIABLE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_VARIABLE_H

#include "game_data.h"
#include "rv_string.h"

namespace mafia::game_types
{
    namespace _private
    {
        class DebugVariable
        {
            //Don't use them...
        public:
            virtual ~DebugVariable() {}

            virtual void get_name(char* buffer, int len) const = 0;
            virtual void* get_val() const = 0;
        };
    }

    class GameVariable: public _private::DebugVariable
    {
    public:
        mafia::game_types::String name;
        GameValue value;
        bool read_only {false};

    public:
        GameVariable();
        GameVariable(String name_, GameValue&& val_, bool read_only_ = false);
        GameVariable(String name_, const GameValue& val_, bool read_only_ = false);

        std::string_view get_map_key() const;
        void get_name(char* buffer, int len) const override;
        void* get_val() const override;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_VARIABLE_H
