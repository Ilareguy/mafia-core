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

#ifndef DEF_MAFIA_CORE_CONTAINERS_MAP_STRING_TO_CLASS_H
#define DEF_MAFIA_CORE_CONTAINERS_MAP_STRING_TO_CLASS_H

#include "../game_types/rv_allocator.h"
#include <string_view>
#include <cctype>

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    static unsigned int rv_map_hash_string_case_sensitive(std::string_view key, int hashValue = 0) noexcept;
    static unsigned int rv_map_hash_string_case_insensitive(std::string_view key, int hashValue = 0) noexcept;

    struct map_string_to_class_trait
    {
        static unsigned int hash_key(std::string_view key) noexcept;
        static bool compare_keys(std::string_view k1, std::string_view k2) noexcept;
    };

    struct map_string_to_class_trait_case_insensitive: map_string_to_class_trait
    {
        static unsigned int hash_key(std::string_view key) noexcept;
        static bool compare_keys(std::string_view k1, std::string_view k2) noexcept;
    };

    template<class Type, class Container, class Traits = map_string_to_class_trait>
    class MapStringToClass
    {
    protected:
        Container* _table = nullptr;
        int _tableCount {0};
        int _count {0};
        static Type _null_entry;

    public:
        class iterator
        {
            size_t _table;
            size_t _item;
            const MapStringToClass <Type, Container, Traits>* _map;

            size_t number_of_tables()
            {
                return _map->_table ? static_cast<size_t>(_map->_tableCount) : 0u;
            }

            void get_next()
            {
                while (_table < number_of_tables() && _item >= _map->_table[_table].count())
                {
                    _table++;
                    _item = 0;
                }
            }

        public:
            iterator(const MapStringToClass <Type, Container, Traits>& base_)
            {
                _table = 0;
                _item = 0;
                _map = &base_;
                get_next();
            }

            iterator(const MapStringToClass <Type, Container, Traits>& base_, bool)
            {  //Creates end Iterator
                _item = 0;
                _map = &base_;
                _table = number_of_tables();
            }

            iterator& operator++()
            {
                if (_table >= number_of_tables())
                { return *this; }
                ++_item;
                get_next();
                return *this;
            }

            iterator operator++(int)
            {
                iterator _tmp = *this;
                ++*this;
                return (_tmp);
            }

            bool operator==(const iterator& _other)
            {
                return _table == _other._table && _item == _other._item;
            }

            bool operator!=(const iterator& _other)
            {
                return _table != _other._table || _item != _other._item;
            }

            const Type& operator*() const
            {
                return _map->_table[_table][_item];
            }

            const Type* operator->() const
            {
                return &_map->_table[_table][_item];
            }

            Type& operator*()
            {
                return _map->_table[_table][_item];
            }

            Type* operator->()
            {
                return &_map->_table[_table][_item];
            }
        };

        iterator begin()
        {
            return iterator(*this);
        }

        iterator end()
        {
            return iterator(*this, true);
        }

        const iterator begin() const
        {
            return iterator(*this);
        }

        const iterator end() const
        {
            return iterator(*this, true);
        }

        MapStringToClass() {}

        template<class Func>
        void for_each(Func func_) const
        {
            if (!_table || !_count)
            { return; }
            for (int i = 0; i < _tableCount; i++)
            {
                const Container& container = _table[i];
                for (int j = 0; j < container.count(); j++)
                {
                    func_(container[j]);
                }
            }
        }

        template<class Func>
        void for_each_backwards(Func func_) const
        {
            if (!_table || !_count)
            { return; }
            for (int i = _tableCount - 1; i >= 0; i--)
            {
                const Container& container = _table[i];
                for (int j = container.count() - 1; j >= 0; j--)
                {
                    func_(container[j]);
                }
            }
        }

        const Type& get(std::string_view key_) const
        {
            if (!_table || !_count)
            { return _null_entry; }
            const int hashed_key = hash_key(key_);
            for (size_t i = 0; i < _table[hashed_key].count(); i++)
            {
                const Type& item = _table[hashed_key][i];
                if (Traits::compare_keys(item.get_map_key(), key_))
                {
                    return item;
                }
            }
            return _null_entry;
        }

        Container* get_table_for_key(std::string_view key_)
        {
            if (!_table || !_count)
            { return nullptr; }
            const int hashed_key = hash_key(key_);
            return &_table[hashed_key];
        }

        const Container* get_table_for_key(std::string_view key_) const
        {
            if (!_table || !_count)
            { return nullptr; }
            const int hashed_key = hash_key(key_);
            return &_table[hashed_key];
        }

        Type& get(std::string_view key_)
        {
            if (!_table || !_count)
            { return _null_entry; }
            const int hashed_key = hash_key(key_);
            for (size_t i = 0; i < _table[hashed_key].count(); i++)
            {
                Type& item = _table[hashed_key][i];
                if (Traits::compare_keys(item.get_map_key(), key_))
                {
                    return item;
                }
            }
            return _null_entry;
        }

        static bool is_null(const Type& value_) { return &value_ == &_null_entry; }

        bool has_key(std::string_view key_) const
        {
            return !is_null(get(key_));
        }

        int count() const { return _count; }

        //ArmaDebugEngine
        void rebuild(int tableSize)
        {
            auto oldTableCount = _tableCount;
            _tableCount = tableSize;
            auto newTable = mafia::game_types::RVAllocator<Container>::create_array(tableSize);
            for (auto i = 0; i < _tableCount; i++)
            {
                auto& container = newTable[i];
                for (Type& item : container)
                {
                    auto hashedKey = hash_key(item.get_map_key());
                    auto it = newTable[hashedKey].emplace(newTable[hashedKey].end(), Type());
                    *it = std::move(item);
                }
            }
            std::swap(_table, newTable);
            mafia::game_types::RVAllocator<Container>::destroy_deallocate(newTable, oldTableCount);
        }

        Type& insert(const Type& value)
        {
            //Check if key already exists
            auto key = value.get_map_key();
            Type& old = get(key);
            if (!is_null(old))
            {
                return old;
            }

            //Are we full?
            if (_count + 1 > (16 * _tableCount))
            {
                int tableSize = _tableCount + 1;
                do
                {
                    tableSize *= 2;
                } while (_count + 1 > (16 * (tableSize - 1)));
                rebuild(tableSize - 1);
            }
            auto hashedkey = hash_key(key);
            auto& x = *(_table[hashedkey].emplace_back(value));
            _count++;
            return x;
        }

        Type& insert(Type&& value)
        {
            //Check if key already exists
            auto key = value.get_map_key();
            Type& old = get(key);
            if (!is_null(old))
            {
                old = std::move(value);
                return old;
            }

            //Are we full?
            if (_count + 1 > (16 * _tableCount))
            {
                int tableSize = _tableCount + 1;
                do
                {
                    tableSize *= 2;
                } while (_count + 1 > (16 * (tableSize - 1)));
                rebuild(tableSize - 1);
            }

            if (!_table)
            {
                _table = mafia::game_types::RVAllocator<Container>::create_array(_tableCount);
            }

            auto hashedkey = hash_key(key);
            auto& x = *(_table[hashedkey].emplace_back(std::move(value)));
            _count++;
            return x;
        }

        bool remove(std::string_view key)
        {
            if (_count <= 0)
            { return false; }

            int hashedKey = hash_key(key);
            for (size_t i = 0; i < _table[hashedKey].size(); i++)
            {
                Type& item = _table[hashedKey][i];
                if (Traits::compare_keys(item.get_map_key(), key) == 0)
                {
                    _table[hashedKey].erase(_table[hashedKey].begin() + i);
                    _count--;
                    return true;
                }
            }
            return false;
        }

        //Is empty?
        bool empty()
        {
            return (!_table || !_count);
        }

    protected:
        int hash_key(std::string_view key_) const
        {
            return Traits::hash_key(key_) % _tableCount;
        }
    };

    template<class Type, class Container, class Traits>
    Type MapStringToClass<Type, Container, Traits>::_null_entry;
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_MAP_STRING_TO_CLASS_H
