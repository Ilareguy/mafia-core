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
#include "../containers/auto_array.h"

namespace mafia::game_types
{
    class ParamArchive;
    class ParamArchiveEntry;
    class ParamArchiveArrayEntry;

    class ParamArchive
    {
    public:
        static GameState* get_game_state();

    public:
        explicit ParamArchive(ParamArchiveEntry* p1);
        virtual ~ParamArchive();

    public:
        //#TODO add SRef class
        ParamArchiveEntry* _p1 {
                mafia::game_types::RVAllocator<ParamArchiveEntry>::create_single()
        };  // pointer to classEntry. vtable something
        int _version {1};// version
        // #TODO is that 64bit on x64?
        uint32_t _p3 {
                0
        }; // be careful with alignment seems to always be 0 for exporting.

        containers::AutoArray<uintptr_t> _parameters; // parameters? on serializing gameData only element is pointer to gameState
        bool _isExporting {true}; // writing data vs loading data

        SerializationReturn serialize(mafia::game_types::String name, SerializeClass& value, int min_version);
        SerializationReturn serialize(
                mafia::game_types::String name,
                mafia::game_types::String& value, int min_version
        );
        SerializationReturn serialize(mafia::game_types::String name, bool& value, int min_version);
        SerializationReturn
        serialize(mafia::game_types::String name, bool& value, int min_version, bool default_value);

        template<class Type>
        SerializationReturn
        serialize(const mafia::game_types::String& name, mafia::Ref<Type>& value, int min_version)
        {
            if (_version < min_version)
            { return SerializationReturn::version_too_new; }
            if (_isExporting || _p3 == 2)
            {
                if (!value)
                { return SerializationReturn::no_error; }

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

                const auto ret = value->serialize(sub_archive);
                if (_isExporting)
                {
                    sub_archive._p1->compress();
                    RVAllocator<ParamArchiveEntry>::destroy_deallocate(sub_archive._p1);
                    sub_archive._p1 = nullptr;
                }
            }
            else
            {
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
                auto val = Type::createFromSerialized(sub_archive);
                if (val)
                { val->serialize(sub_archive); }
                if (_isExporting)
                {
                    sub_archive._p1->compress();
                    RVAllocator<ParamArchiveEntry>::destroy_deallocate(sub_archive._p1);
                    sub_archive._p1 = nullptr;
                }
            }
            return SerializationReturn::no_error;
        }
    };

    class ParamArchiveArrayEntry
    {
    public:
        virtual ~ParamArchiveArrayEntry();
        virtual explicit operator float() const;
        virtual explicit operator int() const;
        virtual explicit operator const mafia::game_types::String() const;
        virtual operator mafia::game_types::String() const;
        virtual operator bool() const;
    };

    class ParamArchiveEntry
    {
    public:
        virtual ~ParamArchiveEntry();

        // Number of entries. count for array and number of class entries otherwise
        virtual int entry_count() const;

        // Don't know what that's used for.
        virtual ParamArchiveEntry* get_entry_by_index(int index_) const;

        //Dunno exactly what this is. on GameData serialize it returns "data"
        virtual String current_entry_name();

        virtual ParamArchiveEntry* get_entry_by_name(const mafia::game_types::String& name) const;

        //Normal Entry. Contains a single value of a single type
        virtual operator float() const;
        virtual operator int() const;
        virtual operator int64_t() const;

    private:
        struct rv_string_dummy: public mafia::game_types::String
        {
            rv_string_dummy(const mafia::game_types::String& o);
        };

        virtual operator const rv_string_dummy() const;

    public:
        virtual operator mafia::game_types::String() const;
        virtual operator bool() const;
        virtual mafia::game_types::String _placeholder1(uint32_t) const;

        //Array entry
        virtual void reserve(int count_) {}

        virtual void add_array_entry(float value_) {}

        virtual void add_array_entry(int value_) {}

        virtual void add_array_entry(int64_t value_) {}

        virtual void add_array_entry(const mafia::game_types::String& value_) {}

        virtual int count() const;

        virtual ParamArchiveArrayEntry* operator[](int index_) const;

        //Class entry (contains named values)
        virtual ParamArchiveEntry* add_entry_array(const mafia::game_types::String& name_);
        virtual ParamArchiveEntry* add_entry_class(const String& name_, bool unknown_ = false);

        virtual void add_entry(const mafia::game_types::String& name_, const mafia::game_types::String& value_) {}

        virtual void add_entry(const mafia::game_types::String& name_, float value_) {}

        virtual void add_entry(const mafia::game_types::String& name_, int value_) {}

        virtual void add_entry(const mafia::game_types::String& name_, int64_t value_) {}

        virtual void compress() {}

        virtual void _placeholder(const mafia::game_types::String& name_) {}
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_PARAM_ARCHIVE_H
