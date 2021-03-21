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

#ifndef DEF_MAFIA_CORE_CONTAINERS_RV_ARRAY_H
#define DEF_MAFIA_CORE_CONTAINERS_RV_ARRAY_H

#include <algorithm>
#include <stdexcept>

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    class RVArrayType
    {
    };

    template<class Type>
    class RVArray: public RVArrayType
    {
    protected:
        Type* _data;
        int _n;

    public:
        class const_iterator;

        class iterator
        {
            friend class const_iterator;
            Type* p_;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = Type;
            using difference_type = ptrdiff_t;
            using pointer = Type*;
            using reference = Type&;

            iterator(): p_(nullptr) {}

            iterator(Type* p): p_(p) {}

            iterator(const iterator& other): p_(other.p_) {}

            iterator& operator=(const iterator& other)
            {
                p_ = other.p_;
                return *this;
            }

            iterator& operator++()
            {
                ++p_;
                return *this;
            }  // prefix++
            iterator operator++(int)
            {
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }  // postfix++
            iterator& operator--()
            {
                --p_;
                return *this;
            }  // prefix--
            iterator operator--(int)
            {
                iterator tmp(*this);
                --(*this);
                return tmp;
            }  // postfix--

            void operator+=(const std::size_t& n) { p_ += n; }

            void operator+=(const iterator& other) { p_ += other.p_; }

            iterator operator+(const std::size_t& n) const
            {
                iterator tmp(*this);
                tmp += n;
                return tmp;
            }

            iterator operator+(const iterator& other) const
            {
                iterator tmp(*this);
                tmp += other;
                return tmp;
            }

            void operator-=(const std::size_t& n) { p_ -= n; }

            void operator-=(const iterator& other) { p_ -= other.p_; }

            iterator operator-(const std::size_t& n) const
            {
                iterator tmp(*this);
                tmp -= n;
                return tmp;
            }

            std::size_t operator-(const iterator& other) const { return p_ - other.p_; }

            bool operator<(const iterator& other) const { return (p_ - other.p_) < 0; }

            bool operator<=(const iterator& other) const { return (p_ - other.p_) <= 0; }

            bool operator>(const iterator& other) const { return (p_ - other.p_) > 0; }

            bool operator>=(const iterator& other) const { return (p_ - other.p_) >= 0; }

            bool operator==(const iterator& other) const { return p_ == other.p_; }

            bool operator!=(const iterator& other) const { return p_ != other.p_; }

            Type& operator*() const { return *p_; }

            Type* operator->() { return p_; }

            operator Type*() { return p_; }
        };
        class const_iterator
        {
            const Type* p_;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = Type;
            using difference_type = ptrdiff_t;
            using pointer = Type*;
            using reference = Type&;

            const_iterator(): p_(nullptr) {}

            const_iterator(const Type* p) noexcept: p_(p) {}

            const_iterator(const typename RVArray<Type>::iterator& other): p_(other.p_) {}

            const_iterator(const const_iterator& other) noexcept: p_(other.p_) {}

            const_iterator& operator=(const const_iterator& other)
            {
                p_ = other.p_;
                return other;
            }

            const_iterator& operator=(const typename RVArray<Type>::iterator& other)
            {
                p_ = other.p_;
                return other;
            }

            const_iterator& operator++() noexcept
            {
                ++p_;
                return *this;
            }  // prefix++
            const_iterator operator++(int)
            {
                const_iterator tmp(*this);
                ++(*this);
                return tmp;
            }  // postfix++
            const_iterator& operator--() noexcept
            {
                --p_;
                return *this;
            }  // prefix--
            const_iterator operator--(int)
            {
                const_iterator tmp(*this);
                --(*this);
                return tmp;
            }  // postfix--

            void operator+=(const std::size_t& n) { p_ += n; }

            void operator+=(const const_iterator& other) { p_ += other.p_; }

            const_iterator operator+(const std::size_t& n) const
            {
                const_iterator tmp(*this);
                tmp += n;
                return tmp;
            }

            const_iterator operator+(const const_iterator& other) const
            {
                const_iterator tmp(*this);
                tmp += other;
                return tmp;
            }

            void operator-=(const std::size_t& n) noexcept { p_ -= n; }

            void operator-=(const const_iterator& other) noexcept { p_ -= other.p_; }

            const_iterator operator-(const std::size_t& n) const
            {
                const_iterator tmp(*this);
                tmp -= n;
                return tmp;
            }

            std::size_t operator-(const const_iterator& other) const noexcept { return p_ - other.p_; }

            bool operator<(const const_iterator& other) const noexcept { return (p_ - other.p_) < 0; }

            bool operator<=(const const_iterator& other) const noexcept { return (p_ - other.p_) <= 0; }

            bool operator>(const const_iterator& other) const noexcept { return (p_ - other.p_) > 0; }

            bool operator>=(const const_iterator& other) const noexcept { return (p_ - other.p_) >= 0; }

            bool operator==(const const_iterator& other) const noexcept { return p_ == other.p_; }

            bool operator!=(const const_iterator& other) const noexcept { return p_ != other.p_; }

            const Type& operator*() const noexcept { return *p_; }

            const Type* operator->() const noexcept { return p_; }

            operator const Type*() const noexcept { return p_; }
        };

        constexpr RVArray() noexcept: _data(nullptr), _n(0) {}

        RVArray(RVArray<Type>&& move_) noexcept: _data(move_._data), _n(move_._n)
        {
            move_._data = nullptr;
            move_._n = 0;
        }

        Type& get(const size_t i_)
        {
            if (i_ > count())
            { throw std::out_of_range("RVArray access out of range"); }
            return _data[i_];
        }

        const Type& get(const size_t i_) const
        {
            if (i_ > count())
            { throw std::out_of_range("RVArray access out of range"); }
            return _data[i_];
        }

        Type& operator[](const size_t i_) { return get(i_); }

        const Type& operator[](const size_t i_) const { return get(i_); }

        Type* data() { return _data; }

        constexpr size_t count() const noexcept { return static_cast<size_t>(_n); }

        constexpr size_t size() const noexcept { return static_cast<size_t>(_n); }

        iterator begin()
        {
            if (!_data)
            { return iterator(); }
            return iterator(&get(0));
        }

        iterator end()
        {
            if (!_data)
            { return iterator(); }
            return iterator(&get(_n));
        }

        const_iterator cbegin() const { return const_iterator(&get(0)); }

        const_iterator cend() const { return const_iterator(&get(_n)); }

        const_iterator begin() const { return const_iterator(&get(0)); }

        const_iterator end() const { return const_iterator(&get(_n)); }

        const Type& front() const { return get(0); }

        const Type& back() const { return get(_n - 1); }

        Type& front() { return get(0); }

        Type& back() { return get(_n - 1); }

        bool is_empty() const { return _n == 0; }

        bool empty() const { return _n == 0; }

        template<class Func>
        void for_each(const Func& f_) const
        {
            for (size_t i = 0; i < count(); i++)
            {
                f_(get(i));
            }
        }

        template<class Func>
        void for_each_backwards(const Func& f_) const
        {  //This returns if Func returns true
            for (size_t i = count() - 1; i >= 0; i--)
            {
                if (f_(get(i)))
                { return; }
            }
        }

        template<class Func>
        void for_each(const Func& f_)
        {
            for (size_t i = 0; i < count(); i++)
            {
                f_(get(i));
            }
        }

        size_t hash() const
        {
            size_t _hash {0};
            for (const auto& it : *this)
            {
                _hash ^= std::hash<Type>()(it) + 0x9e3779b9 + (_hash << 6) + (_hash >> 2);
            }
            return _hash;
        }

        template<typename FindType>
        iterator find(const FindType& find_)
        {
            return std::find(begin(), end(), find_);
        }
    };
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_RV_ARRAY_H
