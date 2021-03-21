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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_INSTRUCTION_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_INSTRUCTION_H

#include "ref_count.h"
#include "./string.h"
#include "game_state.h"
#include "game_value.h"
#include "vm_context.h"

namespace mafia::game_types
{
    class GameInstruction: public mafia::game_types::RefCount
    {
    public:
        // mafia::game_types::Ref<sourcedocposref> sdp;

        virtual bool exec(GameState& state, VMContext& t) = 0;
        virtual int stack_size(void* t) const = 0;
        virtual bool bfunc() const { return false; }

    public:
        virtual mafia::game_types::String get_name() const = 0;
    };

    class GameInstructionConstant: public GameInstruction
    {
    public:
        GameValue value;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_INSTRUCTION_H
