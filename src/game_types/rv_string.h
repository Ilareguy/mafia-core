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

#include "../containers.h"
#include <string_view>

namespace mafia::game_types
{
    // a.k.a. r_string
    class String
    {
    public:
        constexpr String() noexcept = default;

        String(std::string_view str_)
        {
            if (str_.length())
            { _ref = create(str_.data(), str_.length()); }
        }

        explicit String(compact_array<char>* dat_) noexcept:
                _ref(dat_) {}

        String(
                String
                && move_
        ) noexcept
        {
            _ref.swap(move_._ref);
        }

        String(const String& copy_)
        {
            _ref = copy_._ref;
        }

        String& operator=(String&& move_) noexcept
        {
            if (this == &move_)
            {
                return *this;
            }
            _ref.swap(move_._ref);
            return *this;
        }

        String& operator=(const String& copy_) = default;

        String& operator=(std::string_view copy_)
        {
            _ref = create(copy_.data(), copy_.length());
            return *this;
        }

        const char* data() const noexcept
        {
            if (_ref)
            { return _ref->data(); }
            return "";
        }

        const char* c_str() const noexcept { return data(); }

        //This will copy the underlying container if we cannot safely modify this String
        void make_mutable()
        {
            if (_ref && _ref->ref_count() > 1)
            {  //If there is only one reference we can safely modify the string
                _ref = create(_ref->data());
            }
        }

        explicit operator const char*() const noexcept { return data(); }

        operator std::string_view() const noexcept { return std::string_view(data()); }

        //explicit operator std::string() const { return std::string(data()); } //non explicit will break string_view operator because std::string operator because it becomes ambiguous
        ///This calls strlen so O(N)
        size_t length() const noexcept
        {
            if (!_ref)
            { return 0; }
            return strlen(_ref->data());
        }

        ///This calls strlen so O(N)
        size_t size() const noexcept
        {
            return length();
        }

        bool empty() const noexcept
        {
            if (!_ref)
            { return true; }
            return *_ref->data() == '\0';
        }

        size_t capacity() const noexcept
        {
            if (!_ref)
            { return 0; }
            return _ref->size();
        }

        ///== is case insensitive just like scripting
        bool operator==(const char* other_) const
        {
            if (empty())
            { return !other_ || !*other_; }  //empty?

            return operator==(std::string_view(other_));
        }

        ///== is case insensitive just like scripting
        bool operator==(std::string_view other_) const
        {
            if (empty())
            { return other_.empty(); }
            if (other_.length() > _ref->size())
            { return false; }  //There is more data than we can even have

            return std::equal(
                    other_.cbegin(), other_.cend(),
                    begin(), end(),
                    [](unsigned char l, unsigned char r) {
                        return l == r || tolower(l) == tolower(r);
                    }
            );
        }

        ///== is case insensitive just like scripting
        bool operator==(const String& other_) const
        {
            if (empty() || other_.empty())
            { return data() == other_.data(); }
            if (data() == other_.data())
            { return true; }

            return operator==(std::string_view(other_));
        }

        ///== is case insensitive just like scripting
        bool operator==(const std::string& other_) const
        {
            return operator==(std::string_view(other_));
        }

        ///!= is case insensitive just like scripting
        bool operator!=(const String& other_) const
        {
            return !(*this == other_);
        }

        ///!= is case insensitive just like scripting
        bool operator!=(const std::string& other_) const
        {
            return !(*this == other_);
        }

        ///!= is case insensitive just like scripting
        bool operator!=(const std::string_view other_) const
        {  //#TODO templates would be nice here. But we don't want string_view by reference
            return !(*this == other_);
        }

        bool operator<(const String& other_) const
        {
            if (empty())
            { return false; }
            return strcmp(data(), other_.data()) < 0;
        }

        bool operator>(const String& other_) const
        {
            if (empty())
            { return false; }
            return strcmp(data(), other_.data()) > 0;
        }

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
            _t._ref = create(tmp.data(), tmp.length());
            return _in;
        }

        bool compare_case_sensitive(std::string_view other_) const
        {
            if (length() != other_.length())
            { return false; }
            return !std::equal(
                    begin(), end(),
                    other_.cbegin(), [](unsigned char l, unsigned char r) { return l == r; }
            );
        }

        bool compare_case_insensitive(std::string_view other_) const
        {
            if (length() != other_.length())
            { return false; }
            return !std::equal(
                    begin(), end(),
                    other_.cbegin(), [](unsigned char l, unsigned char r) { return ::tolower(l) == ::tolower(r); }
            );
        }

        std::string_view substr(size_t offset, size_t length) const
        {
            if (_ref)
            {
                return std::string_view(data() + offset, std::min(length, size()));
            }
            return std::string_view();
        }

        size_t find(const char ch_, const size_t start_ = 0) const
        {
            if (length() == 0)
            { return -1; }
            const char* pos = strchr(_ref->data() + start_, ch_);
            if (pos == nullptr)
            { return -1; }
            return pos - _ref->data();
        }

        size_t find(std::string_view substring_, const size_t start_ = 0) const
        {
            if (_ref == nullptr || length() == 0 || start_ > length())
            { return -1; }
            auto res = std::search(begin() + start_, end(), substring_.begin(), substring_.end());
            if (res == end())
            { return -1; }
            return std::distance(begin(), res);
        }

        void clear()
        {
            _ref = nullptr;
        }

        size_t hash() const noexcept
        {
            return std::hash<std::string_view>()(std::string_view(data(), _ref ? _ref->size() : 0u));
        }

        String append(std::string_view right_) const
        {
            const auto my_length = length();
            auto new_data = create(my_length + right_.length() + 1);  //Space for terminating nullchar

            std::copy_n(begin(), my_length, new_data->begin());
            std::copy_n(right_.data(), right_.length(), new_data->begin() + my_length);

            new_data->data()[my_length + right_.length()] = 0;
            return String(new_data);
        }

        String& append_modify(std::string_view right_)
        {
            const auto my_length = length();
            auto newData = create(my_length + right_.length() + 1);  //Space for terminating nullchar

            std::copy_n(data(), my_length, newData->begin());
            std::copy_n(right_.data(), right_.length(), newData->begin() + my_length);

            newData->data()[my_length + right_.length()] = 0;
            _ref = newData;
            return *this;
        }

        String operator+(const std::string_view right_) const
        {
            return append(right_);
        }

        friend String operator+(const char* left, const String& right_)
        {
            const auto my_length = strlen(left);
            auto new_data = create(my_length + right_.length() + 1);  //Space for terminating nullchar

            std::copy_n(left, my_length, new_data->begin());
            std::copy_n(right_.data(), right_.length(), new_data->begin() + my_length);

            new_data->data()[my_length + right_.length()] = 0;
            return String(new_data);
        }

        String& operator+=(const std::string_view right_)
        {
            return append_modify(right_);
        }

        String& to_lower()
        {
            if (!_ref)
            { return *this; }
            make_mutable();

            std::transform(_ref->begin(), _ref->end(), _ref->begin(), ::tolower);

            return *this;
        }

        ///Be careful! This returns nullptr on empty string
        compact_array<char>::const_iterator begin() const noexcept
        {
            if (_ref)
            {
                return _ref->begin();
            }
            return {};
        }

        ///Be careful! This returns nullptr on empty string
        compact_array<char>::const_iterator end() const noexcept
        {
            //#TODO could use sentinel here, would spare us the strlen call.
            if (_ref)
            {
                return _ref->begin() +
                       length();
            } //Cannot use compact array end, as that is the whole buffer including null chars or end garbage
            return {};
        }

        char front() const noexcept
        {
            if (_ref)
            {
                return *_ref->begin();
            }
            return '\0';
        }

    private:
        mafia::game_types::Ref<compact_array < char>> _ref;

        static compact_array<char>* create(const char* str, const size_t len_)
        {
            if (len_ == 0 || *str == 0)
            { return nullptr; }
            compact_array<char>* string = compact_array<char>::create(len_ + 1);
#if __GNUC__
            std::copy_n(str, len_, string->data());
#else
            strncpy_s(
                    string->data(), string->size(), str, len_
            );  //#TODO better use memcpy? does strncpy_s check for null chars? we don't want that
#endif
            string->data()[len_] = 0;
            return string;
        }

        static compact_array<char>* create(const size_t len_)
        {
            if (len_ == 0)
            { return nullptr; }
            compact_array<char>* string = compact_array<char>::create(len_ + 1);
            string->data()[0] = 0;
            return string;
        }

        static compact_array<char>* create(const char* str_)
        {
            if (*str_ == 0)
            { return nullptr; }
            return create(str_, strlen(str_));
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_STRING_H
