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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_SQF_SCRIPT_H
#define DEF_MAFIA_CORE_GAME_TYPES_SQF_SCRIPT_H

#include "serialization.h"
#include <cstddef>
#include <set>

namespace mafia::game_types
{
    class ScriptTypeInfo;
    class CompoundScriptTypeInfo;

    typedef std::set<String> ValueTypes_t;
    typedef uintptr_t ValueType_t;

    class SQFScriptType: public mafia::game_types::SerializeClass
    {
    public:
        static uintptr_t type_def;  //#TODO should not be accessible
        SQFScriptType() noexcept { set_vtable(type_def); }

        explicit SQFScriptType(const mafia::game_types::ScriptTypeInfo* type) noexcept;

        //#TODO use type_def instead
        SQFScriptType(
                uintptr_t vt,
                const mafia::game_types::ScriptTypeInfo* st,
                mafia::game_types::CompoundScriptTypeInfo* ct
        ) noexcept;

        void set_vtable(uintptr_t v) noexcept;

        uintptr_t get_vtable() const noexcept { return *reinterpret_cast<const uintptr_t*>(this); }

        SQFScriptType(SQFScriptType&& other) noexcept:
                single_type(other.single_type), compound_type(other.compound_type)
        {
            set_vtable(other.get_vtable());
        }

        SQFScriptType(const SQFScriptType& other) noexcept:
                single_type(other.single_type), compound_type(other.compound_type)
        {
            set_vtable(other.get_vtable());
        }

        SQFScriptType& operator=(const SQFScriptType& other) noexcept
        {
            single_type = other.single_type;
            compound_type = other.compound_type;
            set_vtable(other.get_vtable());
            return *this;
        }

        const ScriptTypeInfo* single_type {nullptr};
        CompoundScriptTypeInfo* compound_type {nullptr};
        ValueTypes_t type() const;
        String type_str() const;

        bool operator==(const SQFScriptType& other) const noexcept
        {
            return single_type == other.single_type && compound_type == other.compound_type;
        }

        bool operator!=(const SQFScriptType& other) const noexcept
        {
            return single_type != other.single_type || compound_type != other.compound_type;
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_SQF_SCRIPT_H
