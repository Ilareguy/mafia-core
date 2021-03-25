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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_DEFS_H
#define DEF_MAFIA_CORE_GAME_TYPES_DEFS_H

#include "v_table.h"
#include "game_value.h"
#include "rv_string.h"
#include "sqf_script_type.h"

namespace mafia::game_types
{
    class GameState;
    class ScriptTypeInfo;

    using GameValueParameter = const game_types::GameValue&;
    using NularFunction = game_types::GameValue (*)(game_types::GameState& state);
    using UnaryFunction = game_types::GameValue (*)(game_types::GameState& state, GameValueParameter);
    using BinaryFunction = game_types::GameValue (*)(
            game_types::GameState& state,
            GameValueParameter,
            GameValueParameter
    );

    void set_game_value_vtable(uintptr_t vtable);

    struct CompoundValuePair
    {
        mafia::game_types::ScriptTypeInfo* first;
        mafia::game_types::ScriptTypeInfo* second;
    };

    struct UnaryOperator: public RefCountDummyVtable
    {
        UnaryFunction* procedure_addr;
        game_types::SQFScriptType return_type;
        game_types::SQFScriptType arg_type;
    };

    struct UnaryEntry
    {
        const char* name;
        uintptr_t procedure_ptr_addr;
        UnaryOperator* op;
    };

    struct BinaryOperator: public RefCountDummyVtable
    {
        BinaryFunction* procedure_addr;
        game_types::SQFScriptType return_type;
        game_types::SQFScriptType arg1_type;
        game_types::SQFScriptType arg2_type;
    };

    struct BinaryEntry
    {
        const char* name;
        uintptr_t procedure_ptr_addr;
        BinaryOperator* op;
    };

    struct NularOperator: public RefCountDummyVtable
    {
        NularFunction* procedure_addr;
        game_types::SQFScriptType return_type;
    };

    struct NularEntry
    {
        const char* name;
        uintptr_t procedure_ptr_addr;
        NularOperator* op;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_DEFS_H
