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

#include "sqf_script_type.h"
#include "script_type_info.h"

using namespace mafia::game_types;

uintptr_t SQFScriptType::type_def {0};

SQFScriptType::SQFScriptType(const mafia::game_types::ScriptTypeInfo* type) noexcept
{
    single_type = type;
    set_vtable(type_def);
}

SQFScriptType::SQFScriptType(
        uintptr_t vt,
        const mafia::game_types::ScriptTypeInfo* st,
        mafia::game_types::CompoundScriptTypeInfo* ct
) noexcept:
        single_type(st), compound_type(ct)
{
    set_vtable(vt);
}

void SQFScriptType::set_vtable(uintptr_t v) noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = v;
}

ValueTypes_t SQFScriptType::type() const
{
    if (single_type != nullptr)
    {
        return {single_type->_name};
    }
    ValueTypes_t x;
    for (auto& it : *compound_type)
    {
        x.insert(it->_name);
    }
    return x;
}

String SQFScriptType::type_str() const
{
    if (single_type != nullptr)
    {
        return single_type->_name;
    }

    return (*compound_type)[0]->_name + "_" + (*compound_type)[1]->_name;
}
