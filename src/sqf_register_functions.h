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
 * File created by Anthony Ilareguy on 24/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_SQF_REGISTER_FUNCTIONS_H
#define DEF_MAFIA_CORE_SQF_REGISTER_FUNCTIONS_H

#include "game_types/game_data_type.h"
#include <cstdint>
#include <vector>

namespace mafia
{
    namespace game_types
    {
        class ScriptTypeInfo;
    }

    struct SQFRegisterFunctions
    {
        SQFRegisterFunctions(): _types(static_cast<size_t>(game_types::GameDataType::end)) {}

        uintptr_t _gameState {};
        uintptr_t _operator_construct {};
        uintptr_t _operator_insert {};
        uintptr_t _unary_construct {};
        uintptr_t _unary_insert {};
        uintptr_t _type_vtable {};
        uintptr_t _compoundtype_vtable {};
        uintptr_t _file_banks {};
        std::vector<const game_types::ScriptTypeInfo*> _types;
    };
}

#endif // DEF_MAFIA_CORE_SQF_REGISTER_FUNCTIONS_H
