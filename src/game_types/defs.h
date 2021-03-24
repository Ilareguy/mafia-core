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

namespace mafia
{
    class RegisteredSQFFunctionImpl;
}

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

    class RegisteredSQFFunction
    {
        // friend class mafia::sqf_functions;

    public:
        constexpr RegisteredSQFFunction() noexcept;
        explicit RegisteredSQFFunction(std::shared_ptr<RegisteredSQFFunctionImpl> func_) noexcept;
        void clear() noexcept;
        bool has_function() const noexcept;

    private:
        std::shared_ptr<RegisteredSQFFunctionImpl> _function;
    };

#if defined _MSC_VER && !defined _WIN64
#pragma warning(disable : 4731)  //ebp was changed in assembly
    template <game_value (*T)(game_value_parameter, game_value_parameter)>
    static game_value userFunctionWrapper(game_state&, game_value_parameter left_arg_, game_value_parameter right_arg_) {
        void* func = (void*)T;
        __asm {
            pop ecx;
            pop ebp;
            mov eax, [esp + 12];
            mov[esp + 8], eax;
            mov eax, [esp + 16];
            mov[esp + 12], eax;
            jmp ecx;
        }
    }

    template <game_value (*T)(game_value_parameter)>
    static game_value userFunctionWrapper(game_state&, game_value_parameter right_arg_) {
        void* func = (void*)T;
        __asm {
            pop ecx;
            pop ebp;
            mov eax, [esp + 12];
            mov[esp + 8], eax;
            jmp ecx;
        }
    }

    template <game_value (*T)()>
    static game_value userFunctionWrapper(game_state&) {
        void* func = (void*)T;
        __asm {
            pop ecx;
            pop ebp;
            jmp ecx;
        }
    }
#pragma warning(default : 4731)  //ebp was changed in assembly
#else

    template<GameValue (* T)(GameValueParameter, GameValueParameter)>
    static GameValue
    userFunctionWrapper(GameState&, GameValueParameter left_arg_, GameValueParameter right_arg_)
    {
        return T(left_arg_, right_arg_);
    }

    template<GameValue (* T)(GameValueParameter)>
    static GameValue userFunctionWrapper(GameState&, GameValueParameter right_arg_)
    {
        return T(right_arg_);
    }

    template<GameValue (* T)()>
    static GameValue userFunctionWrapper(GameState&)
    {
        return T();
    }

#endif

    enum class RegisterPluginInterfaceResult
    {
        success,
        interface_already_registered,
        interface_name_occupied_by_other_module,  //Use list_plugin_interfaces(name_) to find out who registered it
        invalid_interface_class
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_DEFS_H
