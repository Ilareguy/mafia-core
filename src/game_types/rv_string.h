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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_STRING_H
#define DEF_MAFIA_CORE_GAME_TYPES_STRING_H

#include "../containers/compact_array.h"
#include "../ref.h"
#include <string_view>
#include <iostream>
#include <algorithm>
#include <optional>
#include <cstring>
#include <vector>

namespace mafia::game_types
{
    // a.k.a. r_string
    class String
    {
    public:
        constexpr String() noexcept = default;
        String(std::string_view str_);
        explicit String(mafia::containers::CompactArray<char>* dat_) noexcept;
        String(String&& move_) noexcept;
        String(const String& copy_);
        String& operator=(String&& move_) noexcept;
        String& operator=(const String& copy_);
        String& operator=(std::string_view copy_);
        [[nodiscard]] const char* data() const noexcept;
        [[nodiscard]] const char* c_str() const noexcept;
        void make_mutable(); /// This will copy the underlying container if we cannot safely modify this String
        explicit operator const char*() const noexcept;
        operator std::string_view() const noexcept;
        [[nodiscard]] size_t length() const noexcept; /// This calls strlen so O(N)
        [[nodiscard]] size_t size() const noexcept; /// This calls strlen so O(N)
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] size_t capacity() const noexcept;
        bool operator==(const char* other_) const; ///== is case insensitive just like scripting
        bool operator==(std::string_view other_) const; ///== is case insensitive just like scripting
        bool operator==(const String& other_) const; ///== is case insensitive just like scripting
        bool operator==(const std::string& other_) const; ///== is case insensitive just like scripting
        bool operator!=(const String& other_) const; ///!= is case insensitive just like scripting
        bool operator!=(const std::string& other_) const; ///!= is case insensitive just like scripting
        bool operator!=(const std::string_view other_) const; ///!= is case insensitive just like scripting
        bool operator<(const String& other_) const;
        bool operator>(const String& other_) const;
        [[nodiscard]] bool compare_case_sensitive(std::string_view other_) const;
        [[nodiscard]] bool compare_case_insensitive(std::string_view other_) const;
        [[nodiscard]] std::string_view substr(size_t offset, size_t length) const;
        [[nodiscard]] size_t find(const char ch_, const size_t start_ = 0) const;
        [[nodiscard]] size_t find(std::string_view substring_, const size_t start_ = 0) const;
        void clear();
        [[nodiscard]] size_t hash() const noexcept;
        [[nodiscard]] String append(std::string_view right_) const;
        String& append_modify(std::string_view right_);
        String operator+(const std::string_view right_) const;
        friend String operator+(const char* left, const String& right_);
        String& operator+=(const std::string_view right_);
        String& to_lower();
        [[nodiscard]] char front() const noexcept;

        ///Be careful! This returns nullptr on empty string
        mafia::containers::CompactArray<char>::const_iterator begin() const noexcept;
        ///Be careful! This returns nullptr on empty string
        mafia::containers::CompactArray<char>::const_iterator end() const noexcept;

    public:
        friend std::ostream& operator<<(std::ostream& _os, const String& _s)
        {
            _os << _s.data();
            return _os;
        }

        friend std::istream& operator>>(std::istream& _in, String& _t)
        {
            char inp;
            std::string tmp;
            while (_in.get(inp))
            {
                if (inp == 0)
                { break; }
                tmp.push_back(inp);
            }
            _t._ref = String::create(tmp.data(), tmp.length());
            return _in;
        }

    private:
        static mafia::containers::CompactArray<char>* create(const char* str, const size_t len_);
        static mafia::containers::CompactArray<char>* create(const size_t len_);
        static mafia::containers::CompactArray<char>* create(const char* str_);

    private:
        mafia::Ref<mafia::containers::CompactArray<char>> _ref;
    };
}

namespace std
{
    template<>
    struct hash<mafia::game_types::String>
    {
        size_t operator()(const mafia::game_types::String& x) const
        {
            return x.hash();
        }
    };
}  // namespace std

#endif // DEF_MAFIA_CORE_GAME_TYPES_STRING_H
