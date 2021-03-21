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

namespace mafia::game_types
{
    class GameData: public mafia::game_types::RefCount, public _private::I_debug_value
    {
        friend class game_value;
        friend class mafia::invoker;

    public:
        virtual const sqf_script_type& type() const
        {
#ifdef _MSC_VER                  //Only on MSVC on Windows
            __debugbreak();  //If you landed here you did something wrong while implementing your custom type.
#endif
            static sqf_script_type dummy;
            return dummy;
        }

        virtual ~game_data() = default;

    protected:
        virtual bool get_as_bool() const { return false; }

        virtual float get_as_number() const { return 0.f; }

        virtual const r_string& get_as_string() const
        {
            static r_string dummy;
            dummy.clear();
            return dummy;
        }  ///Only usable on String and Code! Use to_string instead!
        virtual const auto_array <game_value>& get_as_const_array() const
        {
            static auto_array <game_value> dummy;
            dummy.clear();
            return dummy;
        }  //Why would you ever need this?
        virtual auto_array <game_value>& get_as_array()
        {
            static auto_array <game_value> dummy;
            dummy.clear();
            return dummy;
        }

    public:
        virtual game_data* copy() const { return nullptr; }

    protected:
        virtual void set_readonly(bool) {}  //No clue what this does...
        virtual bool get_readonly() const { return false; }

        virtual bool get_final() const { return false; }

        virtual void set_final(bool) {}  //Only on GameDataCode AFAIK
    public:
        virtual r_string to_string() const { return r_string(); }

        virtual bool equals(const game_data*) const { return false; }

        virtual const char* type_as_string() const { return "unknown"; }

        virtual bool is_nil() const { return false; }

    private:
        virtual void placeholder() const {}

        virtual bool can_serialize() { return false; }

        int IaddRef() override { return add_ref(); }

        int Irelease() override { return release(); }

    public:  //#TODO make protected and give access to param_archive
        virtual serialization_return serialize(param_archive& ar)
        {
            if (ar._isExporting)
            {
                sqf_script_type _type = type();
                ar.serialize(r_string("type"sv), _type, 1);
            }

            return serialization_return::no_error;
        }

        static game_data* createFromSerialized(param_archive& ar);

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
