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

namespace mafia::game_types
{
    class ScriptTypeInfo
    {  //Donated from ArmaDebugEngine
        using createFunc = GameData* (*)(mafia::game_types::ParamArchive* ar);
#ifdef __linux__
        script_type_info(mafia::game_types::String name, createFunc cf, mafia::game_types::String localizedName, mafia::game_types::String readableName) : _name(std::move(name)), _createFunction(cf), _localizedName(std::move(localizedName)), _readableName(std::move(readableName)), _javaFunc("none") {}
#else

        ScriptTypeInfo(
                mafia::game_types::String name, createFunc cf, mafia::game_types::String localizedName,
                mafia::game_types::String readableName, mafia::game_types::String description,
                mafia::game_types::String category, mafia::game_types::String typeName
        ):
                _name(std::move(name)), _createFunction(cf), _localizedName(std::move(localizedName)),
                _readableName(std::move(readableName)), _description(std::move(description)),
                _category(std::move(category)), _typeName(std::move(typeName)), _javaFunc(
                "none"
        ) {}

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
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_SCRIPT_TYPE_INFO_H
