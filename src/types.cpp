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

#include "types.h"
#include "memory_utility.h"
#include "allocator_info.h"
#include <cassert>
#include <future>
#include <map>

namespace mafia
{

    uintptr_t game_value::__vptr_def;
    uintptr_t sqf_script_type::type_def;

    value_types sqf_script_type::type() const
    {
        if (single_type != nullptr)
        {
            return {single_type->_name};
        }
        value_types x;
        for (auto& it : *compound_type)
        {
            x.insert(it->_name);
        }
        return x;
    }

    r_string sqf_script_type::type_str() const
    {
        if (single_type != nullptr)
        {
            return single_type->_name;
        }

        return (*compound_type)[0]->_name + "_" + (*compound_type)[1]->_name;
    }


    game_data* game_data::createFromSerialized(param_archive& ar)
    {
        bool isNil = false;
        if (ar.serialize(r_string("nil"sv), isNil, 1, false) != serialization_return::no_error)
        {
            return nullptr;
        }

        sqf_script_type _type;
        if (ar.serialize(r_string("type"sv), _type, 1) != serialization_return::no_error)
        { return nullptr; }

        if (isNil)
        {
            //#TODO create GameDataNil or GameDataNothing
            return nullptr;
        }

        auto gs = reinterpret_cast<game_state*>(ar._parameters.front());
        return gs->create_gd_from_type(_type, &ar);
    }

    static std::map<std::string, game_data_type> additionalTypes;

    game_data_type _private::game_datatype_from_string(const r_string& name)
    {
        //I know this is ugly. Feel free to make it better
        if (name == "SCALAR"sv)
        { return game_data_type::SCALAR; }
        if (name == "BOOL"sv)
        { return game_data_type::BOOL; }
        if (name == "ARRAY"sv)
        { return game_data_type::ARRAY; }
        if (name == "STRING"sv)
        { return game_data_type::STRING; }
        if (name == "NOTHING"sv)
        { return game_data_type::NOTHING; }
        if (name == "ANY"sv)
        { return game_data_type::ANY; }
        if (name == "NAMESPACE"sv)
        { return game_data_type::NAMESPACE; }
        if (name == "NaN"sv)
        { return game_data_type::NaN; }
        if (name == "CODE"sv)
        { return game_data_type::CODE; }
        if (name == "OBJECT"sv)
        { return game_data_type::OBJECT; }
        if (name == "SIDE"sv)
        { return game_data_type::SIDE; }
        if (name == "GROUP"sv)
        { return game_data_type::GROUP; }
        if (name == "TEXT"sv)
        { return game_data_type::TEXT; }
        if (name == "SCRIPT"sv)
        { return game_data_type::SCRIPT; }
        if (name == "TARGET"sv)
        { return game_data_type::TARGET; }
        if (name == "CONFIG"sv)
        { return game_data_type::CONFIG; }
        if (name == "DISPLAY"sv)
        { return game_data_type::DISPLAY; }
        if (name == "CONTROL"sv)
        { return game_data_type::CONTROL; }
        if (name == "NetObject"sv)
        { return game_data_type::NetObject; }
        if (name == "SUBGROUP"sv)
        { return game_data_type::SUBGROUP; }
        if (name == "TEAM_MEMBER"sv)
        { return game_data_type::TEAM_MEMBER; }
        if (name == "TASK"sv)
        { return game_data_type::TASK; }
        if (name == "DIARY_RECORD"sv)
        { return game_data_type::DIARY_RECORD; }
        if (name == "LOCATION"sv)
        { return game_data_type::LOCATION; }
        auto found = additionalTypes.find(static_cast<std::string>(name));
        if (found != additionalTypes.end())
        {
            return found->second;
        }
        return game_data_type::end;
    }

    std::string_view _private::to_string(game_data_type type)
    {
        switch (type)
        {
            case game_data_type::SCALAR: return "SCALAR"sv;
            case game_data_type::BOOL: return "BOOL"sv;
            case game_data_type::ARRAY: return "ARRAY"sv;
            case game_data_type::STRING: return "STRING"sv;
            case game_data_type::NOTHING: return "NOTHING"sv;
            case game_data_type::ANY: return "ANY"sv;
            case game_data_type::NAMESPACE: return "NAMESPACE"sv;
            case game_data_type::NaN: return "NaN"sv;
            case game_data_type::CODE: return "CODE"sv;
            case game_data_type::OBJECT: return "OBJECT"sv;
            case game_data_type::SIDE: return "SIDE"sv;
            case game_data_type::GROUP: return "GROUP"sv;
            case game_data_type::TEXT: return "TEXT"sv;
            case game_data_type::SCRIPT: return "SCRIPT"sv;
            case game_data_type::TARGET: return "TARGET"sv;
            case game_data_type::CONFIG: return "CONFIG"sv;
            case game_data_type::DISPLAY: return "DISPLAY"sv;
            case game_data_type::CONTROL: return "CONTROL"sv;
            case game_data_type::SUBGROUP: return "SUBGROUP"sv;
            case game_data_type::TEAM_MEMBER:return "TEAM_MEMBER"sv;
            case game_data_type::TASK: return "TASK"sv;
            case game_data_type::DIARY_RECORD: return "DIARY_RECORD"sv;
            case game_data_type::LOCATION: return "LOCATION"sv;
            default:;
        }
        for (auto& it : additionalTypes)
        {
            if (it.second == type)
            {
                return it.first;
            }
        }
        return "";
    }

    void _private::add_game_datatype(const r_string name_, const game_data_type type_)
    {
        additionalTypes[static_cast<std::string>(name_)] = type_;
    }

    bool exiting = false;
    /// @private
    extern "C" DLLEXPORT void CDECL handle_unload_internal()
    {
        exiting = true;
    }

    game_value_static::~game_value_static()
    {
        if (exiting)
        { data._ref = nullptr; }
    }

    game_state* param_archive::get_game_state()
    {
        auto* allocinfo = mafia::memory_utility::get_allocator();
        return allocinfo->gameState;
    }

    mafia::serialization_return
    param_archive::serialize(r_string name, serialize_class& value, int min_version)
    {
        if (_version < min_version)
        { return serialization_return::version_too_new; }
        param_archive sub_archive(nullptr);
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
            return serialization_return::no_entry;
        }

        auto ret = value.serialize(sub_archive);
        if (_isExporting)
        {
            sub_archive._p1->compress();
            rv_allocator<param_archive_entry>::destroy_deallocate(sub_archive._p1);
            sub_archive._p1 = nullptr;
        }
        return ret;
    }

    serialization_return param_archive::serialize(r_string name, r_string& value, int min_version)
    {
        if (_version < min_version)
        { return serialization_return::version_too_new; }
        if (_isExporting)
        {
            _p1->add_entry(name, value);
        }
        else
        {
            unique_ref<param_archive_entry> entry = _p1->get_entry_by_name(name);
            if (!entry)
            { return serialization_return::no_entry; }
            value = static_cast<r_string>(std::move(
                    *entry
            ));   //#TODO check if entry actually contains the type that we want
            return serialization_return::no_error;
        }
        return serialization_return::no_error;
    }

    serialization_return param_archive::serialize(r_string name, bool& value, int min_version)
    {
        if (_version < min_version)
        { return serialization_return::version_too_new; }
        if (_isExporting)
        {
            _p1->add_entry(name, static_cast<int>(value));//I don't know why.. BI does this..
        }
        else
        {
            unique_ref<param_archive_entry> entry = _p1->get_entry_by_name(name);
            if (!entry)
            { return serialization_return::no_entry; }
            value = static_cast<int>(*entry); //#TODO check if entry actually contains the type that we want
            return serialization_return::no_error;
        }
        return serialization_return::no_error;
    }

    serialization_return param_archive::serialize(r_string name, bool& value, int min_version, bool default_value)
    {
        if (_version < min_version)
        {
            if (!_isExporting)
            { value = default_value; }
            return serialization_return::no_error;
        }
        if (_isExporting && value == default_value)
        { return serialization_return::no_error; }
        const serialization_return err = serialize(name, value, min_version);
        // if the value is not found, load the default one
        if (err == serialization_return::no_entry)
        {
            value = default_value;
            return serialization_return::no_error;
        }
        return err;
    }

    serialization_return game_value::serialize(param_archive& ar)
    {
        if (!data)
        { data = rv_allocator<game_data_nothing>::create_single(); }
        ar.serialize(r_string("data"sv), data, 1);

        if (data && data->get_vtable() == game_data_nothing::type_def)
        { data = nullptr; }
        return serialization_return::no_error;
    }
}