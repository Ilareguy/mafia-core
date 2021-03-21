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

#include "script_type_info.h"

using namespace mafia::game_types;

ScriptTypeInfo::ScriptTypeInfo(
        mafia::game_types::String name,
        ScriptTypeInfo::createFunc cf,
        mafia::game_types::String localizedName,
        mafia::game_types::String readableName,
        mafia::game_types::String description,
        mafia::game_types::String category,
        mafia::game_types::String typeName
):
        _name(std::move(name)), _createFunction(cf), _localizedName(std::move(localizedName)),
        _readableName(std::move(readableName)), _description(std::move(description)),
        _category(std::move(category)), _typeName(std::move(typeName)), _javaFunc("none") {}

void CompoundScriptTypeInfo::set_vtable(uintptr_t v) noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = v;
}

uintptr_t CompoundScriptTypeInfo::get_vtable() const noexcept
{
    return *reinterpret_cast<const uintptr_t*>(this);
}

CompoundScriptTypeInfo::CompoundScriptTypeInfo(const mafia::containers::AutoArray<const ScriptTypeInfo*>& types):
        mafia::containers::AutoArray<const ScriptTypeInfo*>(types)
{
    resize(types.size());
    insert(begin(), types.begin(), types.end());
}
