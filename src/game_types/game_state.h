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

#include "rv_allocator_local.h"
#include "source_doc.h"
#include "rv_string.h"
#include "game_data/namespace.h"
#include "../ref_count.h"
#include "../ref.h"
#include "../containers/map_string_to_class.h"
#include <string>

namespace mafia
{
    class SQFFunctions;

    namespace _private
    {
        class GameFunctions;
        class GameOperators;
        class gsNular;
    }
}

namespace mafia::game_types
{
    class GameVarSpace;
    class VMContext;
    class ScriptTypeInfo;
    class SQFScriptType;
    class ParamArchive;
    class GameData;

    class GameState
    {
        friend class ::mafia::SQFFunctions;
    public:
        class GameEvaluator: public RefCount
        {
        public:
            explicit GameEvaluator(GameVarSpace* var = nullptr);

            GameVarSpace* local;
            int handle;

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
            String _errorMessage;
            SourceDocPosition _errorPosition;

            void operator delete(void* ptr_, std::size_t);

        };

        typedef containers::AutoArray<const ScriptTypeInfo*>
                ScriptTypes_t;
        typedef containers::MapStringToClass<::mafia::_private::GameFunctions,
                                             containers::AutoArray<::mafia::_private::GameFunctions>>
                ScriptFunctions_t;
        typedef containers::MapStringToClass<::mafia::_private::GameOperators,
                                             containers::AutoArray<::mafia::_private::GameOperators>>
                ScriptOperators_t;
        typedef containers::MapStringToClass<::mafia::_private::gsNular,
                                             containers::AutoArray<::mafia::_private::gsNular>>
                ScriptNulars_t;

        enum class namespace_type
        {
            def = 0, //Default
            ui = 1,
            parsing = 2,
            mission = 3
        };

        ///Equivalent to currentNamespace SQF command
        Ref<game_data::Namespace> get_current_namespace(namespace_type type) const;
        Ref<game_data::Namespace> get_global_namespace(namespace_type type) const;

        /**
        * @brief Retrieve a local variable
        * @param name The lowercase name of the variable. Has to be lowercase.
        * @details Walks through the scope's from current to the topmost scope and tries to find a local variable.
        * @return Returns the value of the variable. Returns nil if not found.
        */
        GameValue get_local_variable(std::string_view name) const;

        /**
        * @brief Set a local variable in the current scope
        * @param name The lowercase name of the variable. Has to be lowercase.
        * @param editExisting Check if variable exists in any parent scope, and edit that one (SQF behaviour as without private keyword)\n
            If you don't set editExisting then the variable will still be overwritten if it already exists in the current scope
        */
        void set_local_variable(const String& name, GameValue value, bool editExisting = true) const;

        /**
        * @brief Deletes a local variable in the current scope
        * @param name The lowercase name of the variable. Has to be lowercase.
        */
        void delete_local_variable(std::string_view name);

        /**
        * @brief Sets a script error at current position.
        * @param type This type is actually irrelevant, it just needs to be !=ok and !=handled though it's still recommended to use a sensible type
        */
        void set_script_error(GameEvaluator::evaluator_error_type type, String message);

        /**
        * @brief Sets a script error at custom position.
        * @param type This type is actually irrelevant, it just needs to be !=ok and !=handled though it's still recommended to use a sensible type
        */
        void set_script_error(GameEvaluator::evaluator_error_type type, String message, SourceDocPosition position);

        ///Checks whether value is array of appropriate size, if not it sets the appropriate error message and returns false
        bool error_check_size(GameValue value, size_t min_size);

        ///Checks whether value is of expected type, if not it sets the appropriate error message and returns false
        bool error_check_type(GameValue value, GameDataType expected_type);

        VMContext* get_vm_context() const;
        GameEvaluator* get_evaluator() const;
        const ScriptTypes_t& get_script_types();
        const ScriptFunctions_t& get_script_functions();
        const ScriptOperators_t& get_script_operators();
        const ScriptNulars_t& get_script_nulars();

        GameData* create_game_data_from_type(const SQFScriptType& type, ParamArchive* ar) const;

    private:
        containers::AutoArray<const ScriptTypeInfo*> _scriptTypes;
        ScriptFunctions_t _scriptFunctions;
        ScriptOperators_t _scriptOperators;
        ScriptNulars_t _scriptNulars;
        containers::AutoArray<Ref<GameEvaluator>, RVAllocatorLocal<Ref<GameEvaluator>, 64>> context;
        GameEvaluator* eval;
        Ref<game_data::Namespace> varspace;  //Maybe currentNamespace?

        //0: ? parsing maybe?
        //1: ui
        //2: profile
        //3: mission
        const containers::AutoArray<game_data::Namespace*> namespaces;  //mission/parsing/... namespace
        bool dummy2;
        bool onscreen_script_errors;
        VMContext* current_context;

    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_STATE_H
