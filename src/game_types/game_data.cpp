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

#include "game_data.h"
#include "param_archive.h"

using namespace mafia::game_types;
using namespace std::literals::string_view_literals;

const SQFScriptType& GameData::type() const
{
#ifdef _MSC_VER                  //Only on MSVC on Windows
    __debugbreak();  //If you landed here you did something wrong while implementing your custom type.
#endif
    static SQFScriptType dummy;
    return dummy;
}

GameData::~GameData() = default;

bool GameData::get_as_bool() const { return false; }

float GameData::get_as_number() const { return 0.f; }

const mafia::game_types::String& GameData::get_as_string() const
{
    static mafia::game_types::String dummy;
    dummy.clear();
    return dummy;
}

const mafia::auto_array<GameValue>& GameData::get_as_const_array() const
{
    static auto_array <mafia::game_types::GameValue> dummy;
    dummy.clear();
    return dummy;
}

mafia::auto_array<GameValue>& GameData::get_as_array()
{
    static auto_array <mafia::game_types::GameValue> dummy;
    dummy.clear();
    return dummy;
}

mafia::game_types::GameData* GameData::copy() const { return nullptr; }

void GameData::set_readonly(bool) {}

bool GameData::get_readonly() const { return false; }

bool GameData::get_final() const { return false; }

void GameData::set_final(bool) {}

mafia::game_types::String GameData::to_string() const { return mafia::game_types::String(); }

bool GameData::equals(const mafia::game_types::GameData*) const { return false; }

const char* GameData::type_as_string() const { return "unknown"; }

bool GameData::is_nil() const { return false; }

void GameData::placeholder() const {}

bool GameData::can_serialize() { return false; }

int GameData::IaddRef(){ return add_ref(); }

int GameData::Irelease(){ return release(); }

mafia::game_types::SerializationReturn GameData::serialize(ParamArchive& ar)
{
    if (ar._isExporting)
    {
        SQFScriptType _type = type();
        ar.serialize(String("type"sv), _type, 1);
    }

    return mafia::game_types::SerializationReturn::no_error;
}

mafia::game_types::GameData* GameData::createFromSerialized(ParamArchive& ar)
{
    bool isNil = false;
    if (ar.serialize(String("nil"sv), isNil, 1, false) != SerializationReturn::no_error)
    {
        return nullptr;
    }

    SQFScriptType _type;
    if (ar.serialize(String("type"sv), _type, 1) != SerializationReturn::no_error)
    { return nullptr; }

    if (isNil)
    {
        ///#TODO create GameDataNil or GameDataNothing
        return nullptr;
    }

    auto gs = reinterpret_cast<GameState*>(ar._parameters.front());
    return gs->create_game_data_from_type(_type, &ar);
}
