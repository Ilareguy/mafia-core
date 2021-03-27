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

#ifndef DEF_MAFIA_CORE_SQF_FUNCTIONS_H
#define DEF_MAFIA_CORE_SQF_FUNCTIONS_H

#include "arguments.h"
#include "sqf_register_functions.h"
#include "game_types/game_data.h"
#include "game_types/game_state.h"
#include "game_types/defs.h"
#include "game_types/game_data_type.h"
#include "game_types/rv_string.h"
#include "game_types/script_type_info.h"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <map>
#include <array>
#include <string_view>

namespace mafia
{
    namespace _private
    {
        class gsFuncBase
        {
        public:
            game_types::String _name;

            void copyPH(const gsFuncBase* other) noexcept
            {
                securityStuff = other->securityStuff;
                //std::copy(std::begin(other->securityStuff), std::end(other->securityStuff), std::begin(securityStuff));
            }

        private:
            std::array<size_t,
#if _WIN64 || __X86_64__
                       10
#else
                    #ifdef __linux__
                8
            #else
                11
            #endif
#endif
            > securityStuff {};  //Will scale with x64
            //size_t securityStuff[11];
        };

        class gsFunction: public gsFuncBase
        {
            void* placeholder12 {nullptr};//0x30  //jni function
        public:
            game_types::String _name2;//0x34 this is (tolower name)
            game_types::UnaryOperator* _operator;//0x38
            game_types::String _rightType;//0x3c RString to something
            game_types::String _description;//0x38
            game_types::String _example;
            game_types::String _example2;
            game_types::String placeholder_11;
            game_types::String placeholder_12;
            game_types::String _category {std::string_view {"intercept"}}; //0x48
            //const rv_string* placeholder13;
        };
        class gsOperator: public gsFuncBase
        {
            void* placeholder12 {nullptr};//0x30  JNI function
        public:
            game_types::String _name2;//0x34 this is (tolower name)
            int32_t placeholder_10 {4}; //0x38 Small int 0-5  priority
            game_types::BinaryOperator* _operator;//0x3c
            game_types::String _leftType;//0x40 Description of left hand side parameter
            game_types::String _rightType;//0x44 Description of right hand side parameter
            game_types::String _description;//0x48
            game_types::String _example;//0x4c
            game_types::String placeholder_11;//0x60
            game_types::String _version;//0x64 some version number
            game_types::String placeholder_12;//0x68
            game_types::String _category {std::string_view {"intercept"}}; //0x6c
        };
        class gsNular: public gsFuncBase
        {
        public:
            game_types::String _name2;//0x30 this is (tolower name)
            game_types::NularOperator* _operator;//0x34
            game_types::String _description;//0x38
            game_types::String _example;
            game_types::String _example2;
            game_types::String _version;//0x44 some version number
            game_types::String placeholder_10;
            game_types::String _category; //0x4d
            void* placeholder11 {nullptr};//0x50 JNI probably
            [[nodiscard]] std::string_view get_map_key() const noexcept { return _name2; }
        };

        class GameFunctions: public containers::AutoArray<gsFunction>, public gsFuncBase
        {
        public:
            explicit GameFunctions(game_types::String name): _name(std::move(name)) {}

            game_types::String _name;
            GameFunctions() noexcept = default;

            [[nodiscard]] std::string_view get_map_key() const noexcept { return _name; }
        };

        class GameOperators: public containers::AutoArray<gsOperator>, public gsFuncBase
        {
        public:
            explicit GameOperators(game_types::String name): _name(std::move(name)) {}

            game_types::String _name;
            int32_t placeholder10 {4}; //0x2C Small int 0-5  priority
            GameOperators() noexcept = default;

            [[nodiscard]] std::string_view get_map_key() const noexcept { return _name; }
        };
    }

    namespace game_types
    {
        template class RVAllocator<mafia::_private::gsFunction>;
        template class RVAllocator<mafia::_private::gsOperator>;
        template class RVAllocator<mafia::_private::gsNular>;
        template class RVAllocator<mafia::_private::GameFunctions>;
        template class RVAllocator<mafia::_private::GameOperators>;
    }

    class RegisteredSQFFunctionImpl;

    class RegisteredSQFFunction
    {
        // friend class mafia::sqf_functions;

    public:
        constexpr RegisteredSQFFunction() noexcept = default;
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

    template<game_types::GameValue (* T)(game_types::GameValueParameter, game_types::GameValueParameter)>
    static game_types::GameValue userFunctionWrapper(
            game_types::GameState&,
            game_types::GameValueParameter left_arg_,
            game_types::GameValueParameter right_arg_
    )
    {
        return T(left_arg_, right_arg_);
    }

    template<game_types::GameValue (* T)(game_types::GameValueParameter)>
    static game_types::GameValue userFunctionWrapper(game_types::GameState&, game_types::GameValueParameter right_arg_)
    {
        return T(right_arg_);
    }

    template<game_types::GameValue (* T)()>
    static game_types::GameValue userFunctionWrapper(game_types::GameState&)
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

    namespace _private
    {
        enum class functionType
        {
            sqf_nular,
            sqf_function,
            sqf_operator
        };

        class RegisteredSQFFunctionWrapper
        {
            using GDT = game_types::GameDataType;
        public:
            struct undo_info
            {
                game_types::String _description;

                union
                {
                    game_types::BinaryFunction* _procB;
                    game_types::UnaryFunction* _procU;
                    game_types::NularFunction* _procN;
                };
            };

            RegisteredSQFFunctionWrapper(GDT return_type_, _private::gsNular* func_);
            RegisteredSQFFunctionWrapper(
                    GDT return_type_, GDT left_arg_type_,
                    _private::gsFunction* func_
            );
            RegisteredSQFFunctionWrapper(
                    GDT return_type_, GDT left_arg_type_,
                    GDT right_arg_type_, _private::gsOperator* func_
            );
            RegisteredSQFFunctionWrapper(GDT return_type_, _private::gsNular* func_, undo_info undo);
            RegisteredSQFFunctionWrapper(
                    GDT return_type_, GDT left_arg_type_,
                    _private::gsFunction* func_, undo_info undo
            );
            RegisteredSQFFunctionWrapper(
                    GDT return_type_, GDT left_arg_type_,
                    GDT right_arg_type_, _private::gsOperator* func_, undo_info undo
            );
            void setUnused() noexcept;

            const functionType _type;
            const game_types::String _name;
            union
            {
                _private::gsNular* _nular;
                _private::gsFunction* _func;
                _private::gsOperator* _op;
            };
            const game_types::GameDataType _lArgType;
            const game_types::GameDataType _rArgType;
            const game_types::GameDataType _returnType;
            std::unique_ptr<undo_info> undo;

        };
    }

    class RegisteredSQFFunctionImpl
    {
    public:
        RegisteredSQFFunctionImpl(std::shared_ptr<_private::RegisteredSQFFunctionWrapper> func_) noexcept;
        /*!
        @brief Gets called when Plugin unloads. This replaces the Function that will get unloaded by a dummy function
        */
        ~RegisteredSQFFunctionImpl();

        std::shared_ptr<_private::RegisteredSQFFunctionWrapper> _func;
    };

    /*!
    @brief Takes care of Wrapper functions for registered SQF Functions
    */
    class SQFFunctions
    {
        using GDT = game_types::GameDataType;
        friend class RegisteredSQFFunctionImpl;
    public:
        SQFFunctions() noexcept;
        ~SQFFunctions();
        void initialize() noexcept;
        void setDisabled() noexcept;
        using WrapperFunctionBinary = game_types::BinaryFunction;
        using WrapperFunctionUnary = game_types::UnaryFunction;
        using WrapperFunctionNular = game_types::NularFunction;
        /**
        * @brief Registers a custom SQF Binary Command
        * @param name
        * @param description
        * @param function_ Your function wrapped into [userFunctionWrapper](https://github.com/intercept/intercept/wiki/Registered-Functions)
        * @param return_arg_type A value from mafia::types::game_data_type
        * @param left_arg_type A value from mafia::types::game_data_type
        * @param right_arg_type A value from mafia::types::game_data_type
        * @return A wrapper that should be kept alive as long as the function should be usable
        * @ingroup RSQF
        */
        [[nodiscard]] RegisteredSQFFunction
        register_sqf_function(
                std::string_view name, std::string_view description, WrapperFunctionBinary function_,
                GDT return_arg_type, GDT left_arg_type,
                GDT right_arg_type
        );
        /**
        * @brief Registers a custom SQF Unary Command
        * @param name
        * @param description
        * @param function_ Your function wrapped into [userFunctionWrapper](https://github.com/intercept/intercept/wiki/Registered-Functions)
        * @param return_arg_type A value from mafia::types::game_data_type
        * @param right_arg_type A value from mafia::types::game_data_type
        * @return A wrapper that should be kept alive as long as the function should be usable
        * @ingroup RSQF
        */
        [[nodiscard]] RegisteredSQFFunction
        register_sqf_function(
                std::string_view name, std::string_view description, WrapperFunctionUnary function_,
                GDT return_arg_type, GDT right_arg_type
        );
        /**
        * @brief Registers a custom SQF Nular Command
        * @param name
        * @param description
        * @param function_ Your function wrapped into [userFunctionWrapper](https://github.com/intercept/intercept/wiki/Registered-Functions)
        * @param return_arg_type A value from mafia::types::game_data_type
        * @return A wrapper that should be kept alive as long as the function should be usable
        * @ingroup RSQF
        */
        [[nodiscard]] RegisteredSQFFunction
        register_sqf_function(
                std::string_view name, std::string_view description, WrapperFunctionNular function_,
                GDT return_arg_type
        );


        bool unregister_sqf_function(const std::shared_ptr<_private::RegisteredSQFFunctionWrapper>& shared);

        /**
        * @brief Registers a custom SQF script type
        * @param name
        * @param localizedName
        * @param description
        * @param typeName
        * @param cf
        * @return The resulting game_data_type enum value and a instantiated sqf_script_type
        * @ingroup RSQF
        */
        std::pair<GDT, game_types::SQFScriptType>
        register_sqf_type(
                std::string_view name, std::string_view localizedName, std::string_view description,
                std::string_view typeName, game_types::ScriptTypeInfo::createFunc cf
        );

        /**
        * @brief Registers a custom SQF Compound script type
        * @param types
        * @return a instantiated sqf_script_type
        * @ingroup RSQF
        */
        game_types::SQFScriptType register_compound_sqf_type(containers::AutoArray<GDT> types);

    private:
        [[nodiscard]] _private::gsNular* findNular(std::string name) const;
        [[nodiscard]] _private::gsFunction* findUnary(std::string name, GDT argument_type) const;
        [[nodiscard]] _private::gsOperator*
        findBinary(std::string name, GDT left_argument_type, GDT right_argument_type) const;
        [[nodiscard]] _private::GameOperators* findOperators(std::string name) const;
        [[nodiscard]] _private::GameFunctions* findFunctions(std::string name) const;
        SQFRegisterFunctions _registerFuncs;
        std::map<uintptr_t, containers::AutoArray<char>> _keeper;

        /**
         * \brief If true then we can carelessly modify the script command tables
         */
        bool _canRegister {false};
    };
}

#endif //DEF_MAFIA_CORE_SQF_FUNCTIONS_H
