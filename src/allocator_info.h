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

#ifndef DEF_MAFIA_CORE_ALLOCATOR_INFO_H
#define DEF_MAFIA_CORE_ALLOCATOR_INFO_H

#include "game_types/game_data_type.h"
#include <array>

namespace mafia
{
    namespace game_types
    {
        class GameValue;
        class GameState;
        class String;

        namespace game_data
        {
            class Code;
        }
    }

    struct AllocatorInfo
    {
        uintptr_t genericAllocBase {0};
        uintptr_t poolFuncAlloc {0};
        uintptr_t poolFuncDealloc {0};
        std::array<mafia::game_types::RVPoolAllocator*, static_cast<size_t>(game_types::GameDataType::end)>
                _poolAllocs {};
        game_types::GameValue
        (* evaluate_func)(const game_types::game_data::Code&, void* ns, const game_types::String& name){nullptr};
        game_types::GameState* gameState {nullptr};
        void* reserved1 {nullptr};
        void* reserved2 {nullptr};
        void* reserved3 {nullptr};
    };
}

#endif // DEF_MAFIA_CORE_ALLOCATOR_INFO_H
