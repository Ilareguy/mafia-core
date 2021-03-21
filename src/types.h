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

    namespace _private
    {
        class game_functions;
        class game_operators;
        class gsNular;

        template<typename T, typename U>
        std::size_t pairhash(const T& first, const U& second)
        {
            size_t _hash = std::hash<T>()(first);
            _hash ^= std::hash<U>()(second) + 0x9e3779b9 + (_hash << 6) + (_hash >> 2);
            return _hash;
        }
    }

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

    struct script_type_info
    {  //Donated from ArmaDebugEngine
        using createFunc = game_data* (*)(mafia::game_types::ParamArchive* ar);
#ifdef __linux__
        script_type_info(mafia::game_types::String name, createFunc cf, mafia::game_types::String localizedName, mafia::game_types::String readableName) : _name(std::move(name)), _createFunction(cf), _localizedName(std::move(localizedName)), _readableName(std::move(readableName)), _javaFunc("none") {}
#else

        script_type_info(
                mafia::game_types::String name,
                createFunc cf,
                mafia::game_types::String localizedName,
                mafia::game_types::String readableName,
                mafia::game_types::String description,
                mafia::game_types::String category,
                mafia::game_types::String typeName
        ):
                _name(std::move(name)), _createFunction(cf), _localizedName(std::move(localizedName)),
                _readableName(std::move(readableName)), _description(std::move(description)),
                _category(std::move(category)), _typeName(std::move(typeName)), _javaFunc("none") {}

#endif
        mafia::game_types::String _name;  // SCALAR
        createFunc _createFunction {nullptr};
        mafia::game_types::String _localizedName;  //@STR_EVAL_TYPESCALAR
        mafia::game_types::String _readableName;   //Number
#ifndef __linux__
        mafia::game_types::String _description;  //A real number.
        mafia::game_types::String _category;     //Default
        mafia::game_types::String _typeName;     //float/NativeObject
#endif
        mafia::game_types::String _javaFunc;  //Lcom/bistudio/JNIScripting/NativeObject;
    };

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

struct sourcedocpos: public mafia::game_types::SerializeClass
    {  //See ArmaDebugEngine for more info on this
        mafia::game_types::String sourcefile;
        uint32_t sourceline;
        mafia::game_types::String content;
        uint32_t pos;

    mafia::game_types::SerializationReturn serialize(mafia::game_types::ParamArchive& ar) override
        {
            return mafia::game_types::SerializationReturn::unknown_error;
        }
    };

    struct sourcedocposref: public sourcedocpos, mafia::game_types::RefCount
    {
    };


    class vm_context;
    class game_variable;

    class game_value_conversion_error: public std::runtime_error
    {
    public:
        explicit game_value_conversion_error(const std::string& _Message)
                : runtime_error(_Message) {}

        explicit game_value_conversion_error(const char* _Message)
                : runtime_error(_Message) {}
    };

    class game_value_static: public game_value
    {
    public:
        game_value_static(): game_value() {}

        ~game_value_static();

        game_value_static(const game_value& copy): game_value(copy) {}

        game_value_static(game_value&& move): game_value(move) {}

        game_value_static& operator=(const game_value& copy)
        {
            data = copy.data;
            return *this;
        }
    };

    class I_debug_variable
    {
        //Don't use them...
    public:
        virtual ~I_debug_variable() {}

        virtual void get_name(char* buffer, int len) const = 0;
        virtual void* get_val() const = 0;
    };

    class game_variable: public I_debug_variable
    {
    public:
        mafia::game_types::String name;
        game_value value;
        bool read_only {false};

        game_variable() {}

        game_variable(mafia::game_types::String name_, game_value&& val_, bool read_only_ = false):
                name(std::move(name_)), value(std::move(val_)), read_only(read_only_) {}

        game_variable(mafia::game_types::String name_, const game_value& val_, bool read_only_ = false):
                name(std::move(name_)), value(val_), read_only(read_only_) {}

        std::string_view get_map_key() const { return name; }

        void get_name(char* buffer, int len) const override
        {
            std::copy(
                    name.begin(), std::min(name.begin() + static_cast<size_t>(len), name.end()),
                    compact_array<char>::iterator(buffer));
            buffer[len - 1] = 0;
        }

        void* get_val() const override
        {
            return value.data;
        }
    };

    class game_var_space: public serialize_class
    {
    public:
        map_string_to_class<game_variable, auto_array<game_variable>> variables;
        game_var_space* parent;
        bool dummy;

        game_variable* get_variable(std::string_view varName)
        {
            auto& var = variables.get(varName.data());
            if (!variables.is_null(var))
            {
                return &var;
            }
            if (parent)
            {
                return parent->get_variable(varName);
            }
            return nullptr;
        }

        void set_variable(mafia::game_types::String varName, game_value newValue)
        {
            auto& var = variables.get(varName.data());
            if (!variables.is_null(var))
            {
                var.value = newValue;
            }
            variables.insert(game_variable(varName, newValue));
        }

        void delete_variable(std::string_view varName)
        {
            auto& var = variables.get(varName.data());
            if (!variables.remove(varName) && parent)
            {
                parent->delete_variable(varName);
            }
        }
    };


    class game_instruction;

    class vm_context: public serialize_class
    {
    public:
        class IDebugScope
        {  //ArmaDebugEngine
        public:
            virtual ~IDebugScope() = default;

            virtual const char* getName() const = 0;
            virtual int varCount() const = 0;
            virtual int getVariables(const IDebugVariable** storage, int count) const = 0;
            virtual _private::I_debug_value::RefType EvaluateExpression(const char* code, unsigned int rad) = 0;
            virtual void getSourceDocPosition(char* file, int fileSize, int& line) = 0;
            virtual IDebugScope* getParent() = 0;
        };

        //ArmaDebugEngine. Usual Intercept users won't need this and shouldn't use this
        class callstack_item: public mafia::game_types::RefCount, public IDebugScope
        {
        public:
            callstack_item* _parent;
            game_var_space _varSpace;

            int _stackEndAtStart;
            int _stackEnd;
            mafia::game_types::String _scopeName;

            virtual game_instruction* next(int& d1, const game_state* s) { return nullptr; };

            virtual bool someEH(void* state) { return false; }

            virtual bool someEH2(void* state) { return false; };

            virtual mafia::game_types::String get_type() const = 0;

            virtual serialization_return serialize(param_archive& ar) { return serialization_return::no_error; }

            virtual void on_before_exec() {}
        };


        auto add_callstack_item(mafia::game_types::Ref<callstack_item> newItem)
        {
            return callstack.emplace_back(newItem);
        }

        void throw_script_exception(game_value value)
        {
            exception_state = true;
            exception_value = std::move(value);
        }

        bool is_scheduled() const
        {
            return scheduled;
        }

        bool is_serialization_enabled() const
        {
            return serialenabled;
        }

        void disable_serialization()
        {
            serialenabled = false;
        }

        const sourcedocpos& get_current_position()
        {
            return sdocpos;
        }


        auto_array<mafia::game_types::Ref<callstack_item>,
                   mafia::game_types::RVAllocatorLocal<mafia::game_types::Ref<callstack_item>,
                                                       64>> callstack;  //#TODO check size on x64
        bool serialenabled;                                                                      //disableSerialization -> true, 0x228
        void* dummyu;                                                                            //VMContextBattlEyeMonitor : VMContextCallback

        //const bool is_ui_context; //no touchy
        auto_array<game_value, mafia::game_types::RVAllocatorLocal<game_value, 32>> scriptStack;

        sourcedoc sdoc;

        sourcedocpos sdocpos;  //last instruction pos

        mafia::game_types::String name;  //profiler might like this

        //breakOut
        mafia::game_types::String breakscopename;
        //throw
        game_value exception_value;  //0x4B0
        //breakOut
        game_value breakvalue;
    private:
        uint32_t d[3];
        bool dumm;
        bool dumm2;             //undefined variables allowed?
        const bool scheduled;   //canSuspend 0x4D6
        bool local;
        bool doNil; //undefined variable will be set to nil (unscheduled). If this is false it will throw error
        //throw
        bool exception_state;   //0x4D9
        bool break_;            //0x4DA
        bool breakout;
    };

    class game_data_number: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;
        game_data_number() noexcept;
        game_data_number(float val_) noexcept;
        game_data_number(const game_data_number& copy_);
        game_data_number(game_data_number&& move_) noexcept;
        game_data_number& operator=(const game_data_number& copy_);
        game_data_number& operator=(game_data_number&& move_) noexcept;
        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        float number;

        size_t hash() const
        {
            return _private::pairhash(type_def, number);
        }
        //protected:
        //    static thread_local game_data_pool<game_data_number> _data_pool;
    };

    class game_data_bool: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;
        game_data_bool();
        game_data_bool(bool val_);
        game_data_bool(const game_data_bool& copy_);
        game_data_bool(game_data_bool&& move_) noexcept;
        game_data_bool& operator=(const game_data_bool& copy_);
        game_data_bool& operator=(game_data_bool&& move_) noexcept;
        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        bool val;

        size_t hash() const { return _private::pairhash(type_def, val); }
        //protected:
        //    static thread_local game_data_pool<game_data_bool> _data_pool;
    };

    class game_data_array: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;
        game_data_array();
        game_data_array(size_t size_);
        game_data_array(const std::vector<game_value>& init_);
        game_data_array(const std::initializer_list<game_value>& init_);
        game_data_array(auto_array<game_value>&& init_);
        game_data_array(const game_data_array& copy_);
        game_data_array(game_data_array&& move_) noexcept;
        game_data_array& operator=(const game_data_array& copy_);
        game_data_array& operator=(game_data_array&& move_) noexcept;
        ~game_data_array();
        auto_array<game_value> data;

        auto length() const { return data.count(); }

        size_t hash() const { return _private::pairhash(type_def, data.hash()); }

        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
    };

    class game_data_string: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;
        game_data_string() noexcept;
        explicit game_data_string(const std::string& str_);
        explicit game_data_string(const mafia::game_types::String& str_);
        game_data_string(const game_data_string& copy_);
        game_data_string(game_data_string&& move_) noexcept;
        game_data_string& operator=(const game_data_string& copy_);
        game_data_string& operator=(game_data_string&& move_) noexcept;
        ~game_data_string();
        mafia::game_types::String raw_string;

        size_t hash() const { return _private::pairhash(type_def, raw_string); }

        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        //protected:
        //    static thread_local game_data_pool<game_data_string> _data_pool;
    };

    class game_data_group: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_group() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, group); }

        void* group {};
    };

    class game_data_config: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_config() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, config); }

        void* config {};
        auto_array<mafia::game_types::String> path;
    };

    class game_data_control: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_control() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, control); }

        void* control {};
    };

    class game_data_display: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_display() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, display); }

        void* display {};
    };

    class game_data_location: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_location() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, location); }

        void* location {};
    };

    class game_data_script: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_script() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, script); }

        void* script {};
    };

    class game_data_side: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_side() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, side); }

        void* side {};
    };

    class game_data_rv_text: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_rv_text() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, rv_text); }

        void* rv_text {};
    };

    class game_data_team_member: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_team_member() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, team); }

        void* team {};
    };

    class game_data_namespace: public game_data, public dummy_vtable_class
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_namespace() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, 0); }  //#TODO proper hashing

        map_string_to_class<game_variable, auto_array<game_variable>> _variables;
        mafia::game_types::String _name;
        bool _1;
    };

    class game_data_nothing: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_nothing() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        static size_t hash() { return 0x1337; }
    };

    class game_instruction: public mafia::game_types::RefCount
    {
    public:
        mafia::game_types::Ref<sourcedocposref> sdp;

        virtual bool exec(game_state& state, vm_context& t) = 0;
        virtual int stack_size(void* t) const = 0;

        virtual bool bfunc() const { return false; }

    public:
        virtual mafia::game_types::String get_name() const = 0;
    };

    class game_instruction_constant: public game_instruction
    {
    public:
        game_value value;
    };

    class game_data_code: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;
        static mafia::game_types::RVPoolAllocator* pool_alloc_base;

        game_data_code() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const { return _private::pairhash(type_def, code_string); }

        static void* operator new(std::size_t sz_);
        static void operator delete(void* ptr_, std::size_t sz_);
        mafia::game_types::String code_string;
        auto_array<mafia::game_types::Ref<game_instruction>> instructions;
        bool is_final;
    };

    class game_data_object: public game_data
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

        game_data_object() noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = type_def;
            *reinterpret_cast<uintptr_t*>(static_cast<I_debug_value*>(this)) = data_type_def;
        }

        size_t hash() const
        {
            return _private::pairhash(
                    type_def, reinterpret_cast<uintptr_t>(object
                                                          ? object->object
                                                          : object));
        }

        struct visualState
        {
            //will be false if you called stuff on nullObj
            bool valid {false};
            vector3 _aside;
            vector3 _up;
            vector3 _dir;

            vector3 _position;
            float _scale;
            float _maxScale;

            float _deltaT;
            vector3 _speed;       // speed in world coordinates
            vector3 _modelSpeed;  // speed in model coordinates (updated in Move())
            vector3 _acceleration;
        };
        struct visual_head_pos
        {
            bool valid {false};
            vector3 _cameraPositionWorld;
            vector3 _aimingPositionWorld;
        };
#ifndef __linux__

        //Not compatible yet. Not sure if every will be.
        visualState get_position_matrix() const noexcept
        {
            if (!object || !object->object)
            { return visualState(); }
            const uintptr_t vbase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(object->object) +
                                                                  0xA0);

            struct visState1
            {
                vector3 _aside;
                vector3 _up;
                vector3 _dir;

                vector3 _position;
                float _scale;
                float _maxScale;
            };
            struct visState2
            {
                float _deltaT;
                vector3 _speed;
                vector3 _modelSpeed;
                vector3 _acceleration;
            };

            const auto s1 = reinterpret_cast<const visState1*>(vbase + sizeof(uintptr_t));
            const auto s2 = reinterpret_cast<const visState2*>(vbase + 0x44);

            return visualState {
                    true,
                    s1->_aside,
                    s1->_up,
                    s1->_dir,
                    {s1->_position.x, s1->_position.z, s1->_position.y},
                    s1->_scale,
                    s1->_maxScale,
                    s2->_deltaT,
                    s2->_speed,
                    s2->_modelSpeed,
                    s2->_acceleration
            };
        }

        visual_head_pos get_head_pos() const
        {
            if (!object || !object->object)
            { return visual_head_pos(); }
#if _WIN64 || __X86_64__
            uintptr_t vbase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(object->object) + 0xD0);
#else
            const auto vbase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(object->object) + 0xA0);
#endif
            class v1
            {
                virtual void doStuff() noexcept {}
            };
            class v2: public v1
            {
                void doStuff() noexcept override {}
            };
            v2* v = reinterpret_cast<v2*>(vbase);
            auto& typex = typeid(*v);
#ifdef __GNUC__
            auto test = typex.name();
#else
            const auto test = typex.raw_name();
#endif

            const auto hash = typex.hash_code();
            if (hash !=
                #if _WIN64 || __X86_64__
                0xb57aedbe2fc8b61e
                #else
                0x6d4f3e40
                #endif
                && strcmp(test, ".?AVManVisualState@@") != 0)
            {
                return visual_head_pos();
            }
            const auto s3 = reinterpret_cast<const visual_head_pos*>(vbase +
                                                                     #if _WIN64 || __X86_64__
                                                                     0x168
#else
                    0x114
#endif
            );
            return visual_head_pos();
            return visual_head_pos {
                    true,
                    {s3->_cameraPositionWorld.x, s3->_cameraPositionWorld.z, s3->_cameraPositionWorld.y},
                    {
                            s3->_aimingPositionWorld.x,
                            s3->_aimingPositionWorld.z,
                            s3->_aimingPositionWorld.y
                    }};
        }

#endif
        struct
        {
            uint32_t _x {};
            void* object {};
        } * object {};
    };
    //#TODO add game_data_nothing

    /*namespace _private
    {
        game_data_type game_datatype_from_string(const mafia::game_types::String& name);
        std::string_view to_string(game_data_type type);
        //Not public API!
        void add_game_datatype(mafia::game_types::String name, game_data_type type);

        struct allocatorInfo
        {
            uintptr_t genericAllocBase;
            uintptr_t poolFuncAlloc;
            uintptr_t poolFuncDealloc;
            std::array<mafia::game_types::RVPoolAllocator*, static_cast<size_t>(game_data_type::end)> _poolAllocs;
            game_value (* evaluate_func)(const game_data_code&, void* ns, const mafia::game_types::String& name){nullptr};
            game_state* gameState;
            void* reserved1;
            void* reserved2;
            void* reserved3;
        };
    }*/

    class registered_sqf_function
    {
        friend class mafia::sqf_functions;

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
    struct hash<mafia::mafia::game_types::String>
    {
        size_t operator()(const mafia::mafia::game_types::String& x) const
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
