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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_PARAM_ARCHIVE_H
#define DEF_MAFIA_CORE_GAME_TYPES_PARAM_ARCHIVE_H

#include "rv_allocator.h"
#include "game_state.h"
#include "serialization.h"

namespace mafia::game_types
{
    class ParamArchive;
    class ParamArchiveEntry;
    class ParamArchiveArrayEntry;

    class ParamArchive
    {
    public:
        static GameState* get_game_state();

        ParamArchive(ParamArchiveEntry* p1): _p1(p1)
        {
            _parameters.push_back(
                    reinterpret_cast<uintptr_t>(get_game_state()));
        }

        virtual ~ParamArchive()
        {
            if (_p1)
            { mafia::game_types::RVAllocator<ParamArchiveEntry>::destroy_deallocate(_p1); }
        }

        //#TODO add SRef class
        ParamArchiveEntry* _p1 {
                mafia::game_types::RVAllocator<ParamArchiveEntry>::create_single()
        };  //pointer to classEntry. vtable something
        int _version {1};//version
        //#TODO is that 64bit on x64?
        uint32_t _p3 {
                0
        }; //be careful with alignment seems to always be 0 for exporting.

        auto_array<uintptr_t> _parameters; //parameters? on serializing gameData only element is pointer to gameState
        bool _isExporting {true};//writing data vs loading data
        SerializationReturn serialize(mafia::game_types::String name, SerializeClass& value, int min_version);
        SerializationReturn
        serialize(mafia::game_types::String name, mafia::game_types::String& value, int min_version);
        SerializationReturn serialize(mafia::game_types::String name, bool& value, int min_version);
        SerializationReturn
        serialize(mafia::game_types::String name, bool& value, int min_version, bool default_value);

        template<class Type>
        SerializationReturn
        serialize(const mafia::game_types::String& name, mafia::game_types::Ref<Type>& value, int min_version)
        {
            if (_version < min_version)
            { return serialization_return::version_too_new; }
            if (_isExporting || _p3 == 2)
            {
                if (!value)
                { return serialization_return::no_error; }

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

                const auto ret = value->serialize(sub_archive);
                if (_isExporting)
                {
                    sub_archive._p1->compress();
                    rv_allocator<param_archive_entry>::destroy_deallocate(sub_archive._p1);
                    sub_archive._p1 = nullptr;
                }
            }
            else
            {
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
                auto val = Type::createFromSerialized(sub_archive);
                if (val)
                { val->serialize(sub_archive); }
                if (_isExporting)
                {
                    sub_archive._p1->compress();
                    rv_allocator<param_archive_entry>::destroy_deallocate(sub_archive._p1);
                    sub_archive._p1 = nullptr;
                }
            }
            return serialization_return::no_error;
        }
    };

    class ParamArchiveArrayEntry
    {
    public:
        virtual ~ParamArchiveArrayEntry() = default;

        virtual operator float() const { return 0; }

        virtual operator int() const { return 0; }

        virtual operator const mafia::game_types::String() const { return mafia::game_types::String(); }

        virtual operator mafia::game_types::String() const { return mafia::game_types::String(); }

        virtual operator bool() const { return false; }
    };

    class ParamArchiveEntry
    {
    public:
        virtual ~ParamArchiveEntry() {}

        //Generic stuff for all types
        virtual int
        entry_count() const { return 0; }                                          //Number of entries. count for array and number of class entries otherwise
        virtual ParamArchiveEntry*
        get_entry_by_index(int index_) const { return nullptr; }  //Don't know what that's used for.
        virtual mafia::game_types::String
        current_entry_name() { return mafia::game_types::String(); }                           //Dunno exactly what this is. on GameData serialize it returns "data"
        virtual ParamArchiveEntry* get_entry_by_name(const mafia::game_types::String& name) const { return nullptr; }

        //Normal Entry. Contains a single value of a single type
        virtual operator float() const { return 0; }

        virtual operator int() const { return 0; }

        virtual operator int64_t() const { return 0; }

    private:
        struct rv_string_dummy: public mafia::game_types::String
        {
            rv_string_dummy(const mafia::game_types::String& o): mafia::game_types::String(o) {}
        };

        virtual operator const rv_string_dummy() const
        {
            return operator mafia::game_types::String();
        }

    public:
        virtual operator mafia::game_types::String() const { return mafia::game_types::String(); }

        virtual operator bool() const { return false; }

        virtual mafia::game_types::String _placeholder1(uint32_t) const { return mafia::game_types::String(); }

        //Array entry
        virtual void
        reserve(int count_) {}  //Yes.. This is indeed a signed integer for something that should be unsigned...
        virtual void add_array_entry(float value_) {}

        virtual void add_array_entry(int value_) {}

        virtual void add_array_entry(int64_t value_) {}

        virtual void add_array_entry(const mafia::game_types::String& value_) {}

        virtual int count() const { return 0; }

        virtual ParamArchiveArrayEntry* operator[](int index_) const { return new ParamArchiveArrayEntry(); }

        //Class entry (contains named values)
        virtual ParamArchiveEntry*
        add_entry_array(const mafia::game_types::String& name_) { return new ParamArchiveEntry; }

        virtual ParamArchiveEntry*
        add_entry_class(
                const mafia::game_types::String& name_,
                bool unknown_ = false
        ) { return new ParamArchiveEntry; }

        virtual void add_entry(const mafia::game_types::String& name_, const mafia::game_types::String& value_) {}

        virtual void add_entry(const mafia::game_types::String& name_, float value_) {}

        virtual void add_entry(const mafia::game_types::String& name_, int value_) {}

        virtual void add_entry(const mafia::game_types::String& name_, int64_t value_) {}

        virtual void compress() {}

        virtual void _placeholder(const mafia::game_types::String& name_) {}
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_PARAM_ARCHIVE_H
