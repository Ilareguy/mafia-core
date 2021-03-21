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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H

#include "ref_count.h"
#include "debug_value.h"
#include "serialization.h"
#include "game_value.h"
#include <cstddef>

namespace mafia::game_types
{
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

    class GameData: public mafia::game_types::RefCount, public mafia::game_types::DebugValue
    {
        // friend class game_value;
        // friend class mafia::invoker;

    public:
        virtual const sqf_script_type& type() const
        {
#ifdef _MSC_VER                  //Only on MSVC on Windows
            __debugbreak();  //If you landed here you did something wrong while implementing your custom type.
#endif
            static sqf_script_type dummy;
            return dummy;
        }

        virtual ~GameData() = default;

    protected:
        virtual bool get_as_bool() const { return false; }

        virtual float get_as_number() const { return 0.f; }

        virtual const mafia::game_types::String& get_as_string() const
        {
            static mafia::game_types::String dummy;
            dummy.clear();
            return dummy;
        }  ///Only usable on String and Code! Use to_string instead!
        virtual const auto_array<mafia::game_types::GameValue>& get_as_const_array() const
        {
            static auto_array<mafia::game_types::GameValue> dummy;
            dummy.clear();
            return dummy;
        }  //Why would you ever need this?
        virtual auto_array<mafia::game_types::GameValue>& get_as_array()
        {
            static auto_array<mafia::game_types::GameValue> dummy;
            dummy.clear();
            return dummy;
        }

    public:
        virtual mafia::game_types::GameData* copy() const { return nullptr; }

    protected:
        virtual void set_readonly(bool) {}  //No clue what this does...
        virtual bool get_readonly() const { return false; }

        virtual bool get_final() const { return false; }

        virtual void set_final(bool) {}  //Only on GameDataCode AFAIK
    public:
        virtual mafia::game_types::String to_string() const { return mafia::game_types::String(); }

        virtual bool equals(const mafia::game_types::GameData*) const { return false; }

        virtual const char* type_as_string() const { return "unknown"; }

        virtual bool is_nil() const { return false; }

    private:
        virtual void placeholder() const {}

        virtual bool can_serialize() { return false; }

        int IaddRef() override { return add_ref(); }

        int Irelease() override { return release(); }

    public:  //#TODO make protected and give access to param_archive
        virtual mafia::game_types::SerializationReturn serialize(mafia::game_types::ParamArchive& ar)
        {
            if (ar._isExporting)
            {
                sqf_script_type _type = type();
                ar.serialize(r_string("type"sv), _type, 1);
            }

            return mafia::game_types::SerializationReturn::no_error;
        }

        static mafia::game_types::GameData* createFromSerialized(mafia::game_types::ParamArchive& ar);

    protected:
#ifdef __linux__
        public:
#endif

        uintptr_t get_vtable() const
        {
            return *reinterpret_cast<const uintptr_t*>(this);
        }

        uintptr_t get_secondary_vtable() const noexcept
        {
            return *reinterpret_cast<const uintptr_t*>(static_cast<const _private::I_debug_value*>(this));
        }
    };
{

};
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H
