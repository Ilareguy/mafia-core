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

#include "debug_value.h"
#include "serialization.h"
#include "../ref_count.h"
#include "../containers/auto_array.h"
#include <cstddef>

namespace mafia::game_types
{
    class RVPoolAllocator;
    class SQFScriptType;
    class GameValue;
}

namespace mafia::game_types
{
    class GameData: public mafia::RefCount,
                    public mafia::game_types::DebugValue
    {
        // friend class game_value;
        // friend class mafia::invoker;

    public:
        virtual const SQFScriptType& type() const;
        virtual ~GameData();

    public:
        virtual bool get_as_bool() const;
        virtual float get_as_number() const;
        /// Only usable on String and Code! Use to_string instead!
        virtual const mafia::game_types::String& get_as_string() const;
        /// Why would you ever need this?
        virtual const mafia::containers::AutoArray<mafia::game_types::GameValue>& get_as_const_array() const;
        virtual mafia::containers::AutoArray<mafia::game_types::GameValue>& get_as_array();

    public:
        virtual mafia::game_types::GameData* copy() const;

    protected:
        virtual void set_readonly(bool);
        virtual bool get_readonly() const;
        virtual bool get_final() const;
        virtual void set_final(bool); ///Only on GameDataCode AFAIK
    public:
        virtual mafia::game_types::String to_string() const;
        virtual bool equals(const mafia::game_types::GameData*) const;
        virtual const char* type_as_string() const;
        virtual bool is_nil() const;

    private:
        virtual void placeholder() const;
        virtual bool can_serialize();
        int IaddRef() override;
        int Irelease() override;

    public:
        virtual mafia::game_types::SerializationReturn serialize(mafia::game_types::ParamArchive& ar);
        static mafia::game_types::GameData* createFromSerialized(mafia::game_types::ParamArchive& ar);

#ifdef __linux__
        public:
#endif
        uintptr_t get_vtable() const;
        uintptr_t get_secondary_vtable() const noexcept;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H
