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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_CODE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_CODE_H

#include "../game_data.h"
#include "../game_instruction.h"
#include "../../containers.h"

namespace mafia::game_types::game_data
{
    class Code: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;

    public:
        Code() noexcept;

        size_t hash() const;

        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        mafia::game_types::String code_string;
        auto_array<mafia::game_types::Ref<GameInstruction>> instructions;
        bool is_final;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_CODE_H
