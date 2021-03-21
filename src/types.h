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
 * File created by Anthony Ilareguy on 13/12/2020.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_TYPES_H
#define DEF_MAFIA_CORE_TYPES_H

#include "vector.h"
#include "shared.h"
#include "containers.h"
#include "game_types/game_type.h"
#include "game_types/rv_allocator_local.h"
#include "game_types/rv_pool_allocator.h"
#include "game_types/rv_string.h"
#include "game_types/param_archive.h"
#include <cstdio>
#include <set>
#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
#include <algorithm>
#include <memory>

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

using namespace std::literals::string_view_literals;
class IDebugVariable;

namespace mafia
{
    class loader;
    class sqf_functions;
    class registered_sqf_function_impl;
    class invoker;

    class game_value;
    using game_value_parameter = const game_value&;
    class game_data_array;
    class game_data;
    class game_state;

    using nular_function = game_value (*)(game_state& state);
    using unary_function = game_value (*)(game_state& state, game_value_parameter);
    using binary_function = game_value (*)(game_state& state, game_value_parameter, game_value_parameter);

    typedef std::set<mafia::game_types::String> value_types;
    typedef uintptr_t value_type;
    void set_game_value_vtable(uintptr_t vtable);

    struct compound_value_pair
    {
        script_type_info* first;
        script_type_info* second;
    };

    struct compound_script_type_info: public auto_array<const script_type_info*>, public dummy_vtable_class
    {
    public:
        compound_script_type_info(const auto_array<const script_type_info*>& types)
        {
            resize(types.size());
            insert(begin(), types.begin(), types.end());
        }

        void set_vtable(uintptr_t v) noexcept { *reinterpret_cast<uintptr_t*>(this) = v; }

        uintptr_t get_vtable() const noexcept { return *reinterpret_cast<const uintptr_t*>(this); }
    };

    class sqf_script_type: public mafia::game_types::SerializeClass
    {
    public:
        static uintptr_t type_def;  //#TODO should not be accessible
        sqf_script_type() noexcept { set_vtable(type_def); }

        explicit sqf_script_type(const script_type_info* type) noexcept
        {
            single_type = type;
            set_vtable(type_def);
        }

        //#TODO use type_def instead
        sqf_script_type(uintptr_t vt, const script_type_info* st, compound_script_type_info* ct) noexcept:
                single_type(st), compound_type(ct)
        {
            set_vtable(vt);
        }

        void set_vtable(uintptr_t v) noexcept { *reinterpret_cast<uintptr_t*>(this) = v; }

        uintptr_t get_vtable() const noexcept { return *reinterpret_cast<const uintptr_t*>(this); }

        sqf_script_type(sqf_script_type&& other) noexcept:
                single_type(other.single_type), compound_type(other.compound_type)
        {
            set_vtable(other.get_vtable());
        }

        sqf_script_type(const sqf_script_type& other) noexcept:
                single_type(other.single_type), compound_type(other.compound_type)
        {
            set_vtable(other.get_vtable());
        }

        sqf_script_type& operator=(const sqf_script_type& other) noexcept
        {
            single_type = other.single_type;
            compound_type = other.compound_type;
            set_vtable(other.get_vtable());
            return *this;
        }

        const script_type_info* single_type {nullptr};
        compound_script_type_info* compound_type {nullptr};
        value_types type() const;
        mafia::game_types::String type_str() const;

        bool operator==(const sqf_script_type& other) const noexcept
        {
            return single_type == other.single_type && compound_type == other.compound_type;
        }

        bool operator!=(const sqf_script_type& other) const noexcept
        {
            return single_type != other.single_type || compound_type != other.compound_type;
        }
    };

    struct unary_operator: _refcount_vtable_dummy
    {
        unary_function* procedure_addr;
        sqf_script_type return_type;
        sqf_script_type arg_type;
    };

    struct unary_entry
    {
        const char* name;
        uintptr_t procedure_ptr_addr;
        unary_operator* op;
    };

    struct binary_operator: _refcount_vtable_dummy
    {
        binary_function* procedure_addr;
        sqf_script_type return_type;
        sqf_script_type arg1_type;
        sqf_script_type arg2_type;
    };

    struct binary_entry
    {
        const char* name;
        uintptr_t procedure_ptr_addr;
        binary_operator* op;
    };

    struct nular_operator: _refcount_vtable_dummy
    {
        nular_function* procedure_addr;
        sqf_script_type return_type;
    };

    struct nular_entry
    {
        const char* name;
        uintptr_t procedure_ptr_addr;
        nular_operator* op;
    };

    class registered_sqf_function
    {
        // friend class mafia::sqf_functions;

    public:
        constexpr registered_sqf_function() noexcept = default;
        explicit registered_sqf_function(std::shared_ptr<registered_sqf_function_impl> func_) noexcept;

        void clear() noexcept { _function = nullptr; }

        bool has_function() const noexcept { return _function.get() != nullptr; }

    private:
        std::shared_ptr<registered_sqf_function_impl> _function;
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

    template<game_value (* T)(game_value_parameter, game_value_parameter)>
    static game_value
    userFunctionWrapper(game_state&, game_value_parameter left_arg_, game_value_parameter right_arg_)
    {
        return T(left_arg_, right_arg_);
    }

    template<game_value (* T)(game_value_parameter)>
    static game_value userFunctionWrapper(game_state&, game_value_parameter right_arg_)
    {
        return T(right_arg_);
    }

    template<game_value (* T)()>
    static game_value userFunctionWrapper(game_state&)
    {
        return T();
    }

#endif

    enum class register_plugin_interface_result
    {
        success,
        interface_already_registered,
        interface_name_occupied_by_other_module,  //Use list_plugin_interfaces(name_) to find out who registered it
        invalid_interface_class
    };
}

namespace std
{
    template<>
    struct hash<mafia::game_types::String>
    {
        size_t operator()(const mafia::game_types::String& x) const
        {
            return x.hash();
        }
    };
    template<>
    struct hash<mafia::game_value>
    {
        size_t operator()(const mafia::game_value& x) const
        {
            return x.hash();
        }
    };
}  // namespace std

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_TYPES_H
