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
 * File created by Anthony Ilareguy on 04/04/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_RUNTIME_INVOKER_FUNCTIONS_H
#define DEF_MAFIA_CORE_RUNTIME_INVOKER_FUNCTIONS_H

#include "../game_types/game_data.h"
#include "../game_types/defs.h"
#include "../game_types/script_type_info.h"
#include "../game_types/game_data_type.h"
#include "../sqf_functions.h"
#include "../allocator_info.h"
#include "../containers/auto_array.h"

namespace mafia::runtime
{
    using WrapperFunctionBinary = mafia::game_types::BinaryFunction;
    using WrapperFunctionUnary = mafia::game_types::UnaryFunction;
    using WrapperFunctionNular = mafia::game_types::NularFunction;

    class InvokerFunctions
    {
#ifndef MAFIA_CORE
        friend class client::host;
#endif
        /*!
        @brief Invokes a raw nular SQF function from a nular function pointer.

        @param function_ The mafia::game_types::NularFunction pointer.

        @return rv_mafia::game_types::GameValue The raw returned data from the function.
        */
        mafia::game_types::GameValue (* invoke_raw_nular)(mafia::game_types::NularFunction function_);
        mafia::game_types::GameValue (* invoke_raw_nular_nolock)(mafia::game_types::NularFunction function_);

        /*!
        @brief Invokes a raw unary SQF function from a unary function pointer.

        @param function_ The mafia::game_types::UnaryFunction pointer.
        @param right_arg_ A reference to a mafia::game_types::GameValue argument.

        @return rv_mafia::game_types::GameValue The raw returned data from the function.
        */
        mafia::game_types::GameValue (* invoke_raw_unary)(
                mafia::game_types::UnaryFunction function_,
                const mafia::game_types::GameValue& right_arg_
        );
        mafia::game_types::GameValue (* invoke_raw_unary_nolock)(
                mafia::game_types::UnaryFunction function_,
                const mafia::game_types::GameValue& right_arg_
        );

        /*!
        @brief Invokes a raw binary SQF function from a binary function pointer.

        @param function_ The mafia::game_types::BinaryFunction pointer.
        @param left_arg_ A reference to a mafia::game_types::GameValue argument for the left side.
        @param right_arg_ A reference to a mafia::game_types::GameValue argument for the right side.

        @return rv_mafia::game_types::GameValue The raw returned data from the function.
        */
        mafia::game_types::GameValue
        (* invoke_raw_binary)(
                mafia::game_types::BinaryFunction function_,
                const mafia::game_types::GameValue& left_arg_,
                const mafia::game_types::GameValue& right_arg_
        );
        mafia::game_types::GameValue (* invoke_raw_binary_nolock)(
                mafia::game_types::BinaryFunction function_, const mafia::game_types::GameValue& left_arg_,
                const mafia::game_types::GameValue& right_arg_
        );

        /*!
        @brief Returns type definitions for a given type string.

        @param [in] type_name_ A char string of the type, "ARRAY", "SCALAR", etc.
        @param [out] type_def_ A reference to a uintptr_t to store the type address.
        @param [out] data_type_def_ A reference to a uintptr_t to store the data
        type address.
        */
        void (* get_type_structure)(std::string_view type_name_, uintptr_t& type_def_, uintptr_t& data_type_def_);

        /*!@{
        @brief Returns a function pointer of the type named based on the function name
        and argument signatures.
        */
        mafia::game_types::NularFunction (* get_nular_function)(std::string_view function_name_);

        mafia::game_types::UnaryFunction (* get_unary_function)(std::string_view function_name_);
        mafia::game_types::UnaryFunction (* get_unary_function_typed)(
                std::string_view function_name_,
                std::string_view right_arg_type_
        );

        mafia::game_types::BinaryFunction (* get_binary_function)(std::string_view function_name_);
        mafia::game_types::BinaryFunction (* get_binary_function_typed)(
                std::string_view function_name_, std::string_view left_arg_type_,
                std::string_view right_arg_type_
        );
        //!@}

        void (* invoker_lock)();
        void (* invoker_unlock)();

        /*!
        @brief Get's a pointer to Arma's memory allocator

        This returns a pointer to Arma's internal memory allocator for use by rv_allocator

        @param value_ A pointer to the allocator
        */
        const mafia::AllocatorInfo* (* get_engine_allocator)();

        //#TODO docs
        const mafia::containers::AutoArray<mafia::game_types::String>* (* get_pbo_files_list)();
    private:
        /*!
        @brief Registers SQF Function
        */
        mafia::RegisteredSQFFunction
        (* register_sqf_function)(
                std::string_view name, std::string_view description, WrapperFunctionBinary function_,
                mafia::game_types::GameDataType return_arg_type, mafia::game_types::GameDataType left_arg_type,
                mafia::game_types::GameDataType right_arg_type
        ) {nullptr};
        mafia::RegisteredSQFFunction
        (* register_sqf_function_unary)(
                std::string_view name, std::string_view description,
                WrapperFunctionUnary function_, mafia::game_types::GameDataType return_arg_type,
                mafia::game_types::GameDataType right_arg_type
        ) {nullptr};
        mafia::RegisteredSQFFunction
        (* register_sqf_function_nular)(
                std::string_view name, std::string_view description,
                WrapperFunctionNular function_, mafia::game_types::GameDataType return_arg_type
        ) {
                nullptr
        };
        std::pair<mafia::game_types::GameDataType, mafia::game_types::SQFScriptType>
        (* register_sqf_type)(
                std::string_view name, std::string_view localizedName, std::string_view description,
                std::string_view typeName, mafia::game_types::ScriptTypeInfo::createFunc cf
        ){nullptr};
        mafia::game_types::SQFScriptType
        (* register_compound_sqf_type)(mafia::containers::AutoArray<mafia::game_types::GameDataType> types){nullptr};

        //only reachable through wrapper that also passes module_name
        mafia::RegisterPluginInterfaceResult
        (* register_plugin_interface)(
                mafia::game_types::String module_name_, std::string_view name_, uint32_t api_version_,
                void* interface_class_
        );
        /*!
        @see mafia::extensions::list_plugin_interfaces
        */
        std::pair<mafia::game_types::String, mafia::containers::AutoArray<uint32_t>> (* list_plugin_interfaces)(
                std::string_view name_
        );
        void* (* request_plugin_interface)(
                mafia::game_types::String module_name_,
                std::string_view name_,
                uint32_t api_version_
        );
    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_INVOKER_FUNCTIONS_H
