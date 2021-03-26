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

#ifndef DEF_MAFIA_CORE_LOADER_H
#define DEF_MAFIA_CORE_LOADER_H

#include "arguments.h"
#include "non_copyable.h"
#include "sqf_register_functions.h"
#include "allocator_info.h"
#include "game_types/rv_string.h"
#include "game_types/game_data_type.h"
#include "game_types/defs.h"
#include <unordered_set>
#include <unordered_map>

namespace mafia
{
    namespace game_types
    {
        class ScriptTypeInfo;
    }

    class Invoker;

    typedef std::unordered_map<game_types::String, std::vector<game_types::NularEntry>> NularMap_t;
    typedef std::unordered_map<game_types::String, std::vector<game_types::UnaryEntry>> UnaryMap_t;
    typedef std::unordered_map<game_types::String, std::vector<game_types::BinaryEntry>> BinaryMap_t;

    class Loader: public NonCopyable
    {
        friend class Invoker;
    public:
        Loader();
        virtual ~Loader();

        // Basically equivalent to `loader::do_function_walk()` in
        // https://github.com/intercept/intercept/blob/master/src/host/loader/loader.cpp
        void init(uintptr_t stack_base);

        /*!
        @brief Returns a unary SQF function from the loaders library of found SQF functions.

        Returns a unary SQF function from the loaders library of found SQF functions.

        @param [in] function_name_ The name of the function, all in lowercase.
        @param [out] function_ A reference variable to the unary function.

        @return true if function is found, false if function is not found.

        @todo Throw exception if overloads are found so that unexpected results
        are not encountered.

        @code
        unary_function can_fire;
        bool found = get_function("canfire", can_fire);
        @endcode
        */
        bool get_function(std::string_view function_name_, game_types::UnaryFunction& function_);

        /*!
        @brief Returns a unary SQF function from the loaders library of found
        SQF functions with argument signature.

        Returns a unary SQF function from the loaders library of found SQF functions
        but also takes a argument type in case there are overloaded versions of
        this SQF command available.

        @param [in] function_name_ The name of the function, all in lowercase.
        @param [out] function_ A reference variable to the unary function.
        @param [in] arg_signature_ The type of variable in SQF that the right
        argument is. Must be in all caps, "ARRAY", "SCALAR", "BOOL", "OBJECT", etc.

        @return `true` if function is found, `false` if function is not found.

        @code
        unary_function random_function1;
        bool found1 = get_function("random", random_function1, "SCALAR");
        unary_function random_function2;
        bool found2 = get_function("random", random_function2, "ARRAY");
        @endcode
        */
        bool get_function(
                std::string_view function_name_,
                game_types::UnaryFunction& function_,
                std::string_view arg_signature_
        );

        /*!
        @brief Returns a binary SQF function from the loaders library of found SQF functions.

        Returns a binary SQF function from the loaders library of found SQF functions.

        @param [in] function_name_ The name of the function, all in lowercase.
        @param [out] function_ A reference variable to the binary function.

        @return `true` if function is found, `false` if function is not found.

        @todo Throw exception if overloads are found so that unexpected results
        are not encountered.

        @code
        binary_function set_pos;
        bool found = get_function("setpos", set_pos);
        @endcode
        */
        bool get_function(std::string_view function_name_, game_types::BinaryFunction& function_);

        /*!
        @brief Returns a binary SQF function from the loaders library of found
        SQF functions with argument signature.

        Returns a binary SQF function from the loaders library of found SQF functions
        but also takes a argument type in case there are overloaded versions of
        this SQF command available.

        @param [in] function_name_ The name of the function, all in lowercase.
        @param [out] function_ A reference variable to the binary function.
        @param [in] arg1_signature_ The type of variable in SQF that the left
        argument is. Must be in all caps, "ARRAY", "SCALAR", "BOOL", "OBJECT",
        etc.
        @param [in] arg2_signature_ The type of variable in SQF that the right
        argument is. Must be in all caps, "ARRAY", "SCALAR", "BOOL", "OBJECT",
        etc.

        @return `true` if function is found, `false` if function is not found.

        @code
        binary_function remove_menu_item1;
        bool found1 = get_function("removemenuitem", remove_menu_item1, "CONTROL", "SCALAR");
        binary_function remove_menu_item2;
        bool found2 = get_function("removemenuitem", remove_menu_item2, "CONTROL", "STRING");
        @endcode
        */
        bool get_function(
                std::string_view function_name_,
                game_types::BinaryFunction& function_,
                std::string_view arg1_signature_,
                std::string_view arg2_signature_
        );

        /*!
        @brief Returns a nular SQF function from the loaders library of found SQF functions.

        Returns a nular SQF function from the loaders library of found SQF functions.
        There is no version of this function that takes a argument signature because
        there are no possible arguements for these functions and hence no possible
        overloading.

        @param [in] function_name_ The name of the function, all in lowercase.
        @param [out] function_ A reference variable to the binary function.

        @return `true` if function is found, `false` if function is not found.

        @code
        nular_function player;
        bool found = get_function("player", player);
        @endcode
        */
        bool get_function(std::string_view function_name_, game_types::NularFunction& function_);

        /*!@{
        @brief Return the associated function maps.
        */
        [[nodiscard]] const UnaryMap_t& unary() const;
        [[nodiscard]] const BinaryMap_t& binary() const;
        [[nodiscard]] const NularMap_t& nular() const;
        //!@}

        /*!
        @brief Returns the pointer to the engines allocator functions.
        */
        [[nodiscard]] const AllocatorInfo* get_allocator() const;

        /*!
        @brief Returns function Pointers needed to register SQF Functions
        */
        [[nodiscard]] const SQFRegisterFunctions& get_register_sqf_info() const;

        [[nodiscard]] game_types::GameState* get_game_state();

    protected:
        /*!
        @name Function Maps
        */
        //!@{
        UnaryMap_t _unary_operators;
        BinaryMap_t _binary_operators;
        NularMap_t _nular_operators;
        //!@}

        /*!
        @brief Stores the hooked functions.
        */
        std::unordered_set<uint32_t> _hooked_functions;

        /*!
        @brief Stores the data about the engines allocators.
        */
        AllocatorInfo _allocator;

        /*!
        @brief Stores the data about the Functions needed to register SQF Functions.
        */
        SQFRegisterFunctions _sqf_register_funcs;

        game_types::GameState* _game_state_ptr;
        uintptr_t evaluate_script_function;
        uintptr_t varset_function;

        bool _attached;
        bool _patched;

    private:
        ///Finds game_state on the Stack
        static uintptr_t find_game_state(uintptr_t stack_base);

    };
}

#endif // DEF_MAFIA_CORE_LOADER_H
