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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_SCRIPT_TYPE_INFO_H
#define DEF_MAFIA_CORE_GAME_TYPES_SCRIPT_TYPE_INFO_H

#include "game_data.h"
#include "v_table.h"
#include "../containers/auto_array.h"

namespace mafia::game_types
{
    class ScriptTypeInfo
    {  //Donated from ArmaDebugEngine
    public:
        using createFunc = GameData* (*)(mafia::game_types::ParamArchive* ar);
#ifdef __linux__
        script_type_info(mafia::game_types::String name, createFunc cf, mafia::game_types::String localizedName, mafia::game_types::String readableName) : _name(std::move(name)), _createFunction(cf), _localizedName(std::move(localizedName)), _readableName(std::move(readableName)), _javaFunc("none") {}
#else

        ScriptTypeInfo(
                String name, createFunc cf, String localizedName,
                String readableName, String description,
                String category, String typeName
        );

#endif
        String _name;  // SCALAR
        createFunc _createFunction {nullptr};
        String _localizedName;  //@STR_EVAL_TYPESCALAR
        String _readableName;   //Number
#ifndef __linux__
        String _description;  //A real number.
        String _category;     //Default
        String _typeName;     //float/NativeObject
#endif
        String _javaFunc;  //Lcom/bistudio/JNIScripting/NativeObject;
    };

class CompoundScriptTypeInfo: public containers::AutoArray<const ScriptTypeInfo*>, public DummyVtable
    {
    public:
        explicit CompoundScriptTypeInfo(const containers::AutoArray<const ScriptTypeInfo*>& types);
        void set_vtable(uintptr_t v) noexcept;
        uintptr_t get_vtable() const noexcept;
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_SCRIPT_TYPE_INFO_H
