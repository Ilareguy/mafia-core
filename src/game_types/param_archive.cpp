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
#include "game_state.h"
#include "../allocator_info.h"
#include "../memory_utility.h"

using namespace mafia::game_types;

GameState* ParamArchive::get_game_state()
{
    auto* allocinfo = mafia::memory_utility::get_allocator();
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
        unique_ref<ParamArchiveEntry> entry = _p1->get_entry_by_name(name);
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
        unique_ref<ParamArchiveEntry> entry = _p1->get_entry_by_name(name);
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
