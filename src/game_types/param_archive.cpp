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

#include "param_archive.h"
#include "../mafia.h"
#include "../rv_controller.h"
#include "../loader.h"
#include "../unique_ref.h"

using namespace mafia::game_types;

ParamArchive::ParamArchive(ParamArchiveEntry* p1): _p1(p1)
{
    _parameters.push_back(
            reinterpret_cast<uintptr_t>(get_game_state()));
}

ParamArchive::~ParamArchive()
{
    if (_p1)
    { mafia::game_types::RVAllocator<ParamArchiveEntry>::destroy_deallocate(_p1); }
}

GameState* ParamArchive::get_game_state()
{
    auto* allocinfo = mafia::controller()->get_loader()->get_allocator();
    return allocinfo->gameState;
}

SerializationReturn ParamArchive::serialize(String name, SerializeClass& value, int min_version)
{
    if (_version < min_version)
    { return SerializationReturn::version_too_new; }
    ParamArchive sub_archive(nullptr);
    sub_archive._version = _version;
    sub_archive._p3 = _p3;
    sub_archive._parameters = _parameters;
    sub_archive._isExporting = _isExporting;
    if (_isExporting)
    {
        sub_archive._p1 = _p1->add_entry_class(name, false);
    }
    else
    {
        sub_archive._p1 = _p1->get_entry_by_name(name);
    }

    if (!sub_archive._p1)
    {
        return SerializationReturn::no_entry;
    }

    auto ret = value.serialize(sub_archive);
    if (_isExporting)
    {
        sub_archive._p1->compress();
        RVAllocator<ParamArchiveEntry>::destroy_deallocate(sub_archive._p1);
        sub_archive._p1 = nullptr;
    }
    return ret;
}

SerializationReturn ParamArchive::serialize(String name, String& value, int min_version)
{
    if (_version < min_version)
    { return SerializationReturn::version_too_new; }
    if (_isExporting)
    {
        _p1->add_entry(name, value);
    }
    else
    {
        mafia::UniqueRef<ParamArchiveEntry> entry = _p1->get_entry_by_name(name);
        if (!entry)
        { return SerializationReturn::no_entry; }
        value = static_cast<String>(std::move(
                *entry
        ));   //#TODO check if entry actually contains the type that we want
        return SerializationReturn::no_error;
    }
    return SerializationReturn::no_error;
}

SerializationReturn ParamArchive::serialize(String name, bool& value, int min_version)
{
    if (_version < min_version)
    { return SerializationReturn::version_too_new; }
    if (_isExporting)
    {
        _p1->add_entry(name, static_cast<int>(value));//I don't know why.. BI does this..
    }
    else
    {
        mafia::UniqueRef<ParamArchiveEntry> entry = _p1->get_entry_by_name(name);
        if (!entry)
        { return SerializationReturn::no_entry; }
        value = static_cast<int>(*entry); //#TODO check if entry actually contains the type that we want
        return SerializationReturn::no_error;
    }
    return SerializationReturn::no_error;
}

SerializationReturn ParamArchive::serialize(String name, bool& value, int min_version, bool default_value)
{
    if (_version < min_version)
    {
        if (!_isExporting)
        { value = default_value; }
        return SerializationReturn::no_error;
    }
    if (_isExporting && value == default_value)
    { return SerializationReturn::no_error; }
    const SerializationReturn err = serialize(name, value, min_version);
    // if the value is not found, load the default one
    if (err == SerializationReturn::no_entry)
    {
        value = default_value;
        return SerializationReturn::no_error;
    }
    return err;
}

ParamArchiveArrayEntry::~ParamArchiveArrayEntry() = default;

ParamArchiveArrayEntry::operator float() const { return 0; }

ParamArchiveArrayEntry::operator int() const { return 0; }

ParamArchiveArrayEntry::operator const mafia::game_types::String() const { return mafia::game_types::String(); }

ParamArchiveArrayEntry::operator mafia::game_types::String() const { return mafia::game_types::String(); }

ParamArchiveArrayEntry::operator bool() const { return false; }

ParamArchiveEntry::~ParamArchiveEntry() {}

int ParamArchiveEntry::entry_count() const { return 0; }

ParamArchiveEntry* ParamArchiveEntry::get_entry_by_index(int index_) const { return nullptr; }

String ParamArchiveEntry::current_entry_name() { return mafia::game_types::String(); }

ParamArchiveEntry* ParamArchiveEntry::get_entry_by_name(const String& name) const { return nullptr; }

ParamArchiveEntry::operator float() const { return 0; }

ParamArchiveEntry::operator int() const { return 0; }

ParamArchiveEntry::operator int64_t() const { return 0; }

ParamArchiveEntry::rv_string_dummy::rv_string_dummy(const String& o): mafia::game_types::String(o) {}

ParamArchiveEntry::operator const rv_string_dummy() const
{
    return operator mafia::game_types::String();
}

ParamArchiveEntry::operator mafia::game_types::String() const { return mafia::game_types::String(); }

ParamArchiveEntry::operator bool() const { return false; }

mafia::game_types::String ParamArchiveEntry::_placeholder1(uint32_t) const { return mafia::game_types::String(); }

int ParamArchiveEntry::count() const { return 0; }

ParamArchiveArrayEntry* ParamArchiveEntry::operator[](int index_) const { return new ParamArchiveArrayEntry(); }

ParamArchiveEntry* ParamArchiveEntry::add_entry_array(const String& name_) { return new ParamArchiveEntry; }

ParamArchiveEntry*
ParamArchiveEntry::add_entry_class(const String& name_, bool unknown_) { return new ParamArchiveEntry; }
