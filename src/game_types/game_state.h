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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_STATE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_STATE_H

#include "ref_count.h"
#include "ref.h"

namespace mafia::game_types
{
    class GameState
    {
    public:
        class game_evaluator: public RefCount
        {
        public:
            explicit game_evaluator(game_var_space* var = nullptr)
            {
                local = var;

                static int handleG = 0;
                handle = handleG++;
                _2 = false;
            }

            //ArmaDebugEngine
            game_var_space* local;  // local variables
            int handle;             //

            bool _1 {false};
            bool _2;

            //https://github.com/dedmen/ArmaDebugEngine/blob/6270d5d6a30c948a3768ae1a31703099082b5280/BIDebugEngine/BIDebugEngine/RVClasses.cpp#L190
            //dta languagecore_f.pbo stringtable STR_EVAL_<name>
            enum class evaluator_error_type
            {
                ok,
                gen,                        //Generic error in expression
                expo,                       //Exponent out of range or invalid
                num,                        //Invalid number in expression
                var,                        //Undefined variable in expression: %s
                bad_var,                    //Reserved variable in expression
                div_zero,                   //Zero divisor
                tg90,                       //Tangents of 90 degrees
                openparenthesis,            //Missing (
                closeparenthesis,           //Missing )
                open_brackets,              //Missing [
                close_brackets,             //Missing ]
                open_braces,                //Missing {
                close_braces,               //Missing }
                equ,                        //Missing =
                semicolon,                  //Missing ;
                quote,                      //Missing ""
                single_quote,               //Missing '
                oper,                       //Unknown operator %s
                line_long,                  //Line is too long
                type,                       //Type %s, expected %s
                name_space,                 //Local variable in global space
                dim,                        //%d elements provided, %d expected
                unexpected_closeb,          //unexpected )
                assertion_failed,           //Assertation failed
                halt_function,              //Debugger breakpoint hit
                foreign,                    //Foreign error: %s
                scope_name_defined_twice,   //Scope name defined twice
                scope_not_found,
                invalid_try_block,
                unhandled_exception,        //Unhandled exception: %s
                stack_overflow,
                handled
            };

            evaluator_error_type _errorType;
            r_string _errorMessage;
            sourcedocpos _errorPosition;

            void operator delete(void* ptr_, std::size_t)
            {
                rv_allocator<game_evaluator>::destroy_deallocate(static_cast<game_evaluator*>(ptr_));
            }

        };

        enum class namespace_type
        {
            def = 0, //Default
            ui = 1,
            parsing = 2,
            mission = 3
        };

        ///Equivalent to currentNamespace SQF command
        ref<game_data_namespace> get_current_namespace(namespace_type type) const
        {
            return varspace;
        }


        ref<game_data_namespace> get_global_namespace(namespace_type type) const
        {
            return namespaces[static_cast<int>(type)];
        }

        /**
        * @brief Retrieve a local variable
        * @param name The lowercase name of the variable. Has to be lowercase.
        * @details Walks through the scope's from current to the topmost scope and tries to find a local variable.
        * @return Returns the value of the variable. Returns nil if not found.
        */
        game_value get_local_variable(std::string_view name) const
        {
            if (!eval || !eval->local)
            { return {}; }
            auto var = eval->local->get_variable(name);
            if (!var)
            { return {}; }
            return var->value;
        }

        /**
        * @brief Set a local variable in the current scope
        * @param name The lowercase name of the variable. Has to be lowercase.
        * @param editExisting Check if variable exists in any parent scope, and edit that one (SQF behaviour as without private keyword)\n
            If you don't set editExisting then the variable will still be overwritten if it already exists in the current scope
        */
        void set_local_variable(const r_string& name, game_value value, bool editExisting = true) const
        {
            if (!eval || !eval->local)
            { return; }
            if (editExisting)
            {
                auto var = eval->local->get_variable(name);
                if (var)
                {
                    var->value = std::move(value);
                    return;
                }
            }
            eval->local->set_variable(name, std::move(value));
        }


        /**
        * @brief Deletes a local variable in the current scope
        * @param name The lowercase name of the variable. Has to be lowercase.
        */
        void delete_local_variable(std::string_view name)
        {
            if (!eval || !eval->local)
            { return; }
            eval->local->delete_variable(name);
        }

        /**
        * @brief Sets a script error at current position.
        * @param type This type is actually irrelevant, it just needs to be !=ok and !=handled though it's still recommended to use a sensible type
        */
        void set_script_error(game_evaluator::evaluator_error_type type, r_string message)
        {
            if (!eval)
            { return; } //Don't know why or how this could happen, but better safe than sorry.
            eval->_errorType = type;
            eval->_errorMessage = message;
            if (current_context)
            {
                eval->_errorPosition = current_context->sdocpos;
            }
        }

        /**
        * @brief Sets a script error at custom position.
        * @param type This type is actually irrelevant, it just needs to be !=ok and !=handled though it's still recommended to use a sensible type
        */
        void set_script_error(game_evaluator::evaluator_error_type type, r_string message, sourcedocpos position)
        {
            if (!eval)
            { return; } //Don't know why or how this could happen, but better safe than sorry.
            eval->_errorType = type;
            eval->_errorMessage = message;
            eval->_errorPosition = position;
        }

        ///Checks whether value is array of appropriate size, if not it set's the appropriate error message and returns false
        bool error_check_size(game_value value, size_t min_size)
        {
            if (value.size() >= min_size)
            { return true; }
            auto message =
                    std::to_string(value.size()) + " elements provided, " + std::to_string(min_size) + " expected";

            set_script_error(game_evaluator::evaluator_error_type::dim, static_cast<r_string>(message));
            return false;
        }

        ///Checks whether value is of expected type, if not it sets the appropriate error message and returns false
        bool error_check_type(game_value value, game_data_type expected_type)
        {
            if (value.type_enum() == expected_type)
            { return true; }

            auto actualName = mafia::_private::to_string(value.type_enum());
            auto expectedName = mafia::_private::to_string(expected_type);
            auto message = r_string("Type ") + actualName + ", expected " + expectedName;
            set_script_error(game_evaluator::evaluator_error_type::type, message);
            return false;
        }

        ///I hope you know what you are doing...
        vm_context* get_vm_context() const
        {
            return current_context;
        }

        ///I hope you know what you are doing...
        game_evaluator* get_evaluator() const
        {
            return eval;
        }

        const auto& get_script_types()
        {
            return _scriptTypes;
        }

        const auto& get_script_functions()
        {
            return _scriptFunctions;
        }

        const auto& get_script_operators()
        {
            return _scriptOperators;
        }

        const auto& get_script_nulars()
        {
            return _scriptNulars;
        }

    private:
        auto_array<const script_type_info*> _scriptTypes;

        using game_functions = mafia::_private::game_functions;
        using game_operators = mafia::_private::game_operators;
        using gsNular = mafia::_private::gsNular;

        map_string_to_class<game_functions, auto_array<game_functions>> _scriptFunctions;
        map_string_to_class<game_operators, auto_array<game_operators>> _scriptOperators;
        map_string_to_class<gsNular, auto_array<gsNular>> _scriptNulars;


        auto_array<ref<game_evaluator>, rv_allocator_local<ref<game_evaluator>, 64>> context;

        game_evaluator* eval;

        ref<game_data_namespace> varspace;  //Maybe currentNamespace?

        //0: ? parsing maybe?
        //1: ui
        //2: profile
        //3: mission
        const auto_array<game_data_namespace*> namespaces;  //mission/parsing/... namespace
        bool dummy2;
        bool onscreen_script_errors;
        vm_context* current_context;

        //callstack item types. auto_array of struct {void* to func; r_string name}

        game_data* create_gd_from_type(const sqf_script_type& type, param_archive* ar) const
        {
            for (auto& it : _scriptTypes)
            {
                if (sqf_script_type(it) == type)
                {
                    if (it->_createFunction)
                    { return it->_createFunction(ar); }
                    return nullptr;
                }
            }
            return nullptr;
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_STATE_H
