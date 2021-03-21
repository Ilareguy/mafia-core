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

#ifndef DEF_MAFIA_CORE_CONTAINERS_H
#define DEF_MAFIA_CORE_CONTAINERS_H

#include "game_types/ref_count.h"
#include "game_types/rv_allocator.h"
#include "game_types/rv_pool_allocator.h"
#include "game_types/ref.h"
#include <string_view>
#include <iostream>
#include <algorithm>
#include <optional>
#include <cstring>
#include <vector>

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia
{
    /*namespace _private
    {
        void* rv_allocator_allocate_generic(size_t _size);
        void rv_allocator_deallocate_generic(void* _Ptr);
        void* rv_allocator_reallocate_generic(void* _Ptr, size_t _size);
    }*/

    class game_value_static;

    ///When this goes out of scope. The pointer is deleted. This takes ownership of the pointer
    template<class Type>
    class unique_ref
    {
    protected:
        Type* _ref;

    public:
        unique_ref() { _ref = NULL; }

        unique_ref(Type* source)
        {
            _ref = source;
        }

        unique_ref(const unique_ref& other)
        {
            _ref = other._ref;
            other._ref = nullptr;  //We take ownership
        }

        ~unique_ref() { clear(); }

        unique_ref& operator=(Type* other)
        {
            if (_ref == other)
            { return *this; }
            clear();
            _ref = other;
            return *this;
        }

        unique_ref& operator=(const unique_ref& other)
        {
            if (other._ref == _ref)
            { return *this; }
            clear();
            _ref = other._ref;
            other._ref = nullptr;  //We take ownership
            return *this;
        }

        bool is_null() const { return _ref == nullptr; }

        void clear()
        {
            if (_ref)
            { delete _ref; }
            _ref = nullptr;
        }

        Type* operator->() const { return _ref; }

        explicit operator Type*() const { return _ref; }

        Type* get() const { return _ref; }
    };



    template<class Type, class Allocator = mafia::game_types::RVAllocator<char>>  //Has to be allocator of type char
    class compact_array: public mafia::game_types::RefCountBase
    {
        //static_assert(std::is_literal_type<Type>::value, "Type must be a literal type");
    public:
        class const_iterator;

        class iterator
        {
            friend class const_iterator;
            Type* p_;

        public:
            using iterator_category_t = std::random_access_iterator_tag;
            using value_t = Type;
            using difference_t = ptrdiff_t;
            using pointer_t = Type*;
            using reference_t = Type&;
            using unchecked_t = pointer_t;

            iterator(): p_(nullptr) {}

            explicit iterator(Type* p): p_(p) {}

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

            difference_t operator-(const iterator& other) const { return p_ - other.p_; }

            bool operator<(const iterator& other) const { return (p_ - other.p_) < 0; }

            bool operator<=(const iterator& other) const { return (p_ - other.p_) <= 0; }

            bool operator>(const iterator& other) const { return (p_ - other.p_) > 0; }

            bool operator>=(const iterator& other) const { return (p_ - other.p_) >= 0; }

            bool operator==(const iterator& other) const { return p_ == other.p_; }

            bool operator!=(const iterator& other) const { return p_ != other.p_; }

            Type& operator*() const { return *p_; }

            Type* operator->() { return p_; }

            explicit operator Type*() { return p_; }
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
            using unchecked_t = pointer;

            const_iterator(): p_(nullptr) {}

            explicit const_iterator(const Type* p) noexcept: p_(p) {}

            const_iterator(const typename compact_array<Type>::iterator& other): p_(other.p_) {}

            const_iterator(const const_iterator& other) noexcept: p_(other.p_) {}

            const_iterator& operator=(const const_iterator& other)
            {
                p_ = other.p_;
                return *this;
            }

            const_iterator& operator=(const typename compact_array<Type>::iterator& other)
            {
                p_ = other.p_;
                return *this;
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

            difference_type operator-(const const_iterator& other) const noexcept { return p_ - other.p_; }

            bool operator<(const const_iterator& other) const noexcept { return (p_ - other.p_) < 0; }

            bool operator<=(const const_iterator& other) const noexcept { return (p_ - other.p_) <= 0; }

            bool operator>(const const_iterator& other) const noexcept { return (p_ - other.p_) > 0; }

            bool operator>=(const const_iterator& other) const noexcept { return (p_ - other.p_) >= 0; }

            bool operator==(const const_iterator& other) const noexcept { return p_ == other.p_; }

            bool operator!=(const const_iterator& other) const noexcept { return p_ != other.p_; }

            const Type& operator*() const noexcept { return *p_; }

            const Type* operator->() const noexcept { return p_; }

            explicit operator const Type*() const noexcept { return p_; }
        };

        size_t size() const noexcept { return _size; }

        Type* data() noexcept { return &_data; }

        const Type* data() const noexcept { return &_data; }

        Type& front() noexcept { return _data; }

        const Type& front() const noexcept { return _data; }

        iterator begin() noexcept { return iterator(&_data); }

        iterator end() noexcept { return iterator((&_data) + _size); }

        const_iterator cbegin() const noexcept { return const_iterator(&_data); }

        const_iterator cend() const noexcept { return const_iterator((&_data) + _size); }

        const Type& operator[](const size_t index_) const
        {
            return *(cbegin() + index_);
        }

        const Type& get(const size_t index_) const
        {
            return *(cbegin() + index_);
        }

        //We delete ourselves! After release no one should have a pointer to us anymore!
        int release() const
        {
            const auto ret = dec_ref();
            if (!ret)
            { del(); }
            return ret;
        }

        //Doesn't clear memory. use create_zero if you want that
        static compact_array* create(size_t number_of_elements_)
        {
            const size_t size = sizeof(compact_array) + sizeof(Type) * (number_of_elements_ -
                                                                        1);  //-1 because we already have one element in compact_array
            auto* buffer = reinterpret_cast<compact_array*>(Allocator::allocate(size));
#pragma warning(suppress : 26409)  //don't use new/delete
            new(buffer) compact_array(number_of_elements_);
            return buffer;
        }

        static compact_array* create_zero(size_t number_of_elements_)
        {
            const size_t size = sizeof(compact_array) + sizeof(Type) * (number_of_elements_ -
                                                                        1);  //-1 because we already have one element in compact_array
            auto* buffer = reinterpret_cast<compact_array*>(Allocator::allocate(size));
#pragma warning(suppress : 26409)  //don't use new/delete
            std::memset(buffer, 0, size);
            new(buffer) compact_array(number_of_elements_);
            return buffer;
        }

        static compact_array* create(const compact_array& other)
        {
            const size_t sizeElements = other.size();
            const size_t sizeBytes = sizeof(compact_array) + sizeof(Type) * (sizeElements - 1);
            auto* buffer = reinterpret_cast<compact_array*>(Allocator::allocate(sizeBytes));
            new(buffer) compact_array(sizeElements);
            std::copy(other.data(), other.data() + other.size(), buffer->data());
            return buffer;
        }

        template<class _InIt>
        static compact_array* create(_InIt beg, _InIt end)
        {
            const size_t sizeElements = (std::distance(beg, end));
            const size_t sizeBytes = sizeof(compact_array) + sizeof(Type) * (sizeElements - 1);
            auto* buffer = reinterpret_cast<compact_array*>(Allocator::allocate(sizeBytes));
            std::memset(buffer, 0, sizeBytes);
#pragma warning(suppress : 26409)  //don't use new/delete
            new(buffer) compact_array(sizeElements);

            size_t index = 0;
            for (auto& i = beg; i < end; ++i)
            {
                buffer->data()[index++] = *i;
            }

            return buffer;
        }

        //Specialty function to copy less elements or to allocate more space
        static compact_array* create(const compact_array& other, size_t element_count)
        {
            const size_t size = other.size();
            auto buffer = reinterpret_cast<compact_array*>(Allocator::allocate(
                    sizeof(compact_array) + sizeof(Type) * (element_count - 1)));
            new(buffer) compact_array(element_count);
            std::memset(buffer->data(), 0, element_count * sizeof(Type));
            const auto elements_to_copy = std::min(size, element_count);

            std::copy(other.cbegin(), other.cbegin() + elements_to_copy, buffer->begin());

            return buffer;
        }

        void del() const
        {
            this->~compact_array();
            const void* _thisptr = this;
#pragma warning(suppress : 26492)  //don't cast away const
            const auto _thisptr2 = const_cast<void*>(_thisptr);
            Allocator::deallocate(static_cast<char*>(_thisptr2), _size - 1 + sizeof(compact_array));
        }

        size_t _size;
        Type _data {};

    private:
        explicit compact_array(const size_t size_) noexcept
        {
            _size = size_;
        }
    };

    //Contributed by ArmaDebugEngine
    class rv_arraytype
    {
    };

    template<class Type>
    class rv_array: public rv_arraytype
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

            explicit iterator(Type* p): p_(p) {}

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

            explicit operator Type*() { return p_; }
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

            explicit const_iterator(const Type* p) noexcept: p_(p) {}

            const_iterator(const typename rv_array<Type>::iterator& other): p_(other.p_) {}

            const_iterator(const const_iterator& other) noexcept: p_(other.p_) {}

            const_iterator& operator=(const const_iterator& other)
            {
                p_ = other.p_;
                return other;
            }

            const_iterator& operator=(const typename rv_array<Type>::iterator& other)
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

            explicit operator const Type*() const noexcept { return p_; }
        };

        constexpr rv_array() noexcept: _data(nullptr), _n(0) {}

        rv_array(rv_array<Type>&& move_) noexcept: _data(move_._data), _n(move_._n)
        {
            move_._data = nullptr;
            move_._n = 0;
        }

        Type& get(const size_t i_)
        {
            if (i_ > count())
            { throw std::out_of_range("rv_array access out of range"); }
            return _data[i_];
        }

        const Type& get(const size_t i_) const
        {
            if (i_ > count())
            { throw std::out_of_range("rv_array access out of range"); }
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

    template<class Type, class Allocator = mafia::game_types::RVAllocator<Type>, size_t growthFactor = 32>
    class
#ifdef _MSC_VER
            __declspec(empty_bases)
#endif
        //#TODO try GCC maybe __attribute__((__packed__)) or #pragma pack. But be careful of the bool in local alloc
    auto_array: public rv_array<Type>,
                private Allocator
    {
        typedef rv_array<Type> base;

    protected:
        int _maxItems;

        Type* try_realloc(Type* old_, size_t n_)
        {
            Type* ret = Allocator::reallocate(old_, n_);
            return ret;
        }

        void reallocate(size_t size_)
        {
            if (_maxItems == size_)
            { return; }

            if (base::_n > static_cast<int>(size_))
            {
                resize(size_);
                return;  //resize calls reallocate and reallocates... Ugly.. I know
            }
            Type* newData = nullptr;
            if (base::_data && size_ > 0 && ((newData = try_realloc(base::_data, size_))))
            {
                //if (size > _maxItems)//Don't null out new stuff if there is no new stuff
                //    std::fill(reinterpret_cast<uint32_t*>(&newData[_maxItems]), reinterpret_cast<uint32_t*>(&newData[size]), 0);
                _maxItems = static_cast<int>(size_);
                base::_data = newData;
                return;
            }
            newData = Allocator::create_uninitialized_array(size_);
            //memset(newData, 0, size * sizeof(Type));
            if (base::_data)
            {
#ifdef __GNUC__
                memmove(newData, base::_data, base::_n * sizeof(Type));
#else
                //std::uninitialized_move(begin(), end(), newData); //This might be cleaner. But still causes a move construct call where memmove just moves bytes.
                memmove_s(newData, size_ * sizeof(Type), base::_data, base::_n * sizeof(Type));
#endif
                Allocator::deallocate(base::_data);
            }
            base::_data = newData;
            _maxItems = static_cast<int>(size_);
        }

        void grow(const size_t n_)
        {
#undef max
            reallocate(_maxItems + std::max(n_, growthFactor));
        }

    public:
        using base::begin;
        using base::cbegin;
        using base::cend;
        using base::end;
        using iterator = typename base::iterator;
        using const_iterator = typename base::const_iterator;

        constexpr auto_array() noexcept: rv_array<Type>(), _maxItems(0) {}

        auto_array(const std::initializer_list<Type>& init_): rv_array<Type>(), _maxItems(0)
        {
            insert(end(), init_.begin(), init_.end());
        }

        template<class _InIt>
        auto_array(_InIt first_, _InIt last_) : rv_array<Type>(), _maxItems(0)
        {
            insert(end(), first_, last_);
        }

        auto_array(const auto_array<Type>& copy_): rv_array<Type>(), _maxItems(0)
        {
            insert(end(), copy_.begin(), copy_.end());
        }

        auto_array(auto_array<Type>&& move_) noexcept: rv_array<Type>(std::move(move_)), _maxItems(move_._maxItems)
        {
            move_._maxItems = 0;
        }

        ~auto_array()
        {
            if (base::_data == nullptr)
            { return; }
            resize(0);
        }

        /**
        * @brief Reduces capacity to minimum required to store current content
        */
        void shrink_to_fit()
        {
            resize(base::_n);
        }

        auto_array& operator=(auto_array&& move_) noexcept
        {
            base::_n = move_._n;
            _maxItems = move_._maxItems;
            base::_data = move_._data;
            move_._data = nullptr;
            move_._n = 0;
            move_._maxItems = 0;
            return *this;
        }

        auto_array& operator=(const auto_array& copy_)
        {
            if (copy_._n)
            {
                clear();
                insert(base::end(), copy_.begin(), copy_.end());
            }
            return *this;
        }

        /**
        * @brief Makes sure the capacity is exactly _n. Destructs and deallocates all elements past n_
        */
        void resize(const size_t n_)
        {
            if (static_cast<int>(n_) < base::_n)
            {
                for (int i = static_cast<int>(n_); i < base::_n; i++)
                {
                    (*this)[i].~Type();
                }
            }
            if (n_ == 0 && base::_data)
            {
                Allocator::deallocate(rv_array<Type>::_data);
                base::_n = 0;
                rv_array<Type>::_data = nullptr;
                return;
            }
            reallocate(n_);
            if (n_ > base::_n)
            { //adding elements, need to default init
                for (size_t i = base::_n; i < n_; ++i)
                {
#pragma warning(suppress : 26409)
                    ::new(base::_data + i) Type();
                }
            }
            base::_n = static_cast<int>(n_);
        }

        /**
        * @brief Makes sure the capacity is big enough to contain res_ elements
        * @param res_ new minimum buffer size
        */
        void reserve(const size_t res_)
        {
            if (_maxItems < static_cast<int>(res_))
            {
                grow(res_ - _maxItems);
            }
        }

        /**
        * @brief Constructs a value at where_
        * @param where_ the iterator where to start inserting
        * @param val_ the arguments passed to the values constructor
        * @return A iterator pointing to the inserted value
        */
        template<class... _Valty>
        iterator emplace(iterator where_, _Valty&& ... val_)
        {
            if (where_ < base::begin() || where_ > base::end())
            { throw std::runtime_error("Invalid Iterator"); }  //WTF?!
            const size_t insertOffset = where_ - base::begin();
            auto previousEnd = base::_n;
            if (_maxItems < base::_n + 1)
            {
                grow(1);
            }
            auto& item = base::_data[base::_n];
            ::new(&item) Type(std::forward<_Valty>(val_)...);
            ++base::_n;

            std::rotate(base::begin() + insertOffset, base::begin() + previousEnd, base::end());

            return base::begin() + insertOffset;
        }

        /**
        * @brief Constructs a new value at the end of the array
        * @param val_ the arguments passed to the values constructor
        * @return A iterator pointing to the inserted value
        */
        template<class... _Valty>
        iterator emplace_back(_Valty&& ... val_)
        {
            if (_maxItems < base::_n + 1)
            {
                grow(1);
            }
            auto& item = base::_data[base::_n];
            ::new(&item) Type(std::forward<_Valty>(val_)...);
            ++base::_n;
            return iterator(&item);
        }

        /**
        * @brief Adds a new value at the end of the array
        * @param val_ the value to be copied into the array
        * @return A iterator pointing to the inserted value
        */
        iterator push_back(const Type& val_)
        {
            return emplace_back(val_);
        }

        /**
        * @brief Adds a new value at the end of the array
        * @param val_ the value to be moved into the array
        * @return A iterator pointing to the inserted value
        */
        iterator push_back(Type&& val_)
        {
            return emplace_back(std::move(val_));
        }

        //void erase(int index) {
        //    if (index > base::_n) return;
        //    auto item = (*this)[index];
        //    item.~Type();
        //    memmove_s(&(*this)[index], (base::_n - index) * sizeof(Type), &(*this)[index + 1], (base::_n - index - 1) * sizeof(Type));
        //}
        void erase(const_iterator element_)
        {
            if (element_ < base::begin() || element_ > base::end())
            { throw std::runtime_error("Invalid Iterator"); }
            const size_t index = std::distance(base::cbegin(), element_);
            if (static_cast<int>(index) > base::_n)
            { return; }
            auto&& item = (*this)[index];
            item.~Type();
#ifdef __GNUC__
            memmove(&(*this)[index], &(*this)[index + 1], (base::_n - index - 1) * sizeof(Type));
#else
            memmove_s(
                    &(*this)[index], (base::_n - index) * sizeof(Type), &(*this)[index + 1],
                    (base::_n - index - 1) * sizeof(Type));
#endif
            --base::_n;
        }

        void erase(const_iterator first_, const_iterator last_)
        {
            if (first_ > last_ || first_ < base::begin() || last_ > base::end())
            {
                throw std::runtime_error(
                        "Invalid Iterator"
                );
            }
            const size_t firstIndex = std::distance(base::cbegin(), first_);
            const size_t lastIndex = std::distance(base::cbegin(), last_);
            const size_t range = std::distance(first_, last_);

            for (size_t index = firstIndex; index < lastIndex; ++index)
            {
                auto item = (*this)[index];
                item.~Type();
            }
            if (last_ != end())
            {
#ifdef __GNUC__
                memmove(&(*this)[firstIndex], &(*this)[lastIndex + 1], (base::_n - lastIndex - 1) * sizeof(Type));
#else
                memmove_s(
                        &(*this)[firstIndex], (base::_n - firstIndex) * sizeof(Type), &(*this)[lastIndex + 1],
                        (base::_n - lastIndex - 1) * sizeof(Type));
#endif
            }
            base::_n -= range;
        }

        void erase(const uint32_t index_, const uint32_t count_ = 1)
        {
            if (count_ == 1)
            {
                erase(begin() + index_);
            }
            else if (count_ > 1)
            {
                erase(begin() + index_, begin() + index_ + (count_ - 1));
            }
        }

        /**
        * @brief Inserts a single value at _where
        * @param _value the value to insert
        * @return A iterator pointing to the inserted value
        */
        template<class _InType>
        //This is sooo not threadsafe!
        iterator insert(iterator _where, _InType&& _value)
        {
            if (_where < base::begin() || _where > base::end())
            { throw std::runtime_error("Invalid Iterator"); }  //WTF?!
            const size_t insertOffset = std::distance(base::begin(), _where);
            const size_t previousEnd = static_cast<size_t>(base::_n);
            const size_t oldSize = base::count();
            reserve(oldSize + 1);

            //emplace_back(_value);
            //custom inlined version of emplace_back. No capacity checks and only incrementing _n once.
            auto& item = base::_data[base::_n];
            ::new(&item) Type(std::forward<decltype(_value)>(_value));
            ++base::_n;

            std::rotate(base::begin() + insertOffset, base::begin() + previousEnd, base::end());
            return base::begin() + insertOffset;
        }

        /**
        * @brief Inserts a range of values at _where
        * @param _first start of the range
        * @param _last end of the range
        * @return A iterator pointing to the first inserted value
        */
        template<class _InIt>
        //This is sooo not threadsafe!
        iterator insert(iterator _where, _InIt _first, _InIt _last)
        {
            if (_first == _last)
            { return _where; }                                                                //Boogie!
            if (_where < base::begin() || _where > base::end())
            { throw std::runtime_error("Invalid Iterator"); }  //WTF?!
            const size_t insertOffset = std::distance(base::begin(), _where);
            const size_t previousEnd = static_cast<size_t>(base::_n);
            const size_t oldSize = base::count();
            const size_t insertedSize = std::distance(_first, _last);
            reserve(oldSize + insertedSize);

            auto index = base::_n;
            for (; _first != _last; ++_first)
            {
                //emplace_back(*_first);
                //custom inlined version of emplace_back. No capacity checks and only incrementing _n once.
                auto& item = base::_data[index];
                ::new(&item) Type(std::forward<decltype(*_first)>(*_first));
                ++index;
            }
            base::_n = index;

            std::rotate(base::begin() + insertOffset, base::begin() + previousEnd, base::end());
            return base::begin() + insertOffset;
        }

        /**
        * @brief Inserts a "range" of values at _where
        * @param where_ the iterator where to start inserting
        * @param values_ the iterator where to start inserting
        * @return A iterator pointing to the first inserted element
        */
        iterator insert(iterator where_, const std::initializer_list<Type>& values_)
        {
            return insert(where_, values_.begin(), values_.end());
        }

        void clear()
        {
            if (base::_data)
            {
                Allocator::deallocate(rv_array<Type>::_data);
            }
            base::_data = nullptr;
            base::_n = 0;
            _maxItems = 0;
        }

        bool operator==(rv_array<Type> other_)
        {
            if (other_._n != base::_n ||
                ((base::_data == nullptr || other_._data == nullptr) && base::_data != other_._data))
            {
                return false;
            }
            auto index = 0;
            for (auto& it : other_)
            {
                if ((*this[index]) != it)
                {
                    return false;
                }
                ++index;
            }
            return true;
        }
    };

    template<class Type, class Allocator = mafia::game_types::RVAllocator<Type>>
    class stack_array: public auto_array<Type, Allocator>
    {
        using base = auto_array<Type, Allocator>;

    public:
        void push(const Type& src) { base::push_back(src); }

        const Type& top() const { return *(base::end() - 1); }

        void pop() { base::erase(base::end() - 1); }
    };

    template<class Type>
    struct find_key_array_traits
    {
        using key_type = const Type&;

        static bool equals(key_type a_, key_type b_) { return a_ == b_; }

        static key_type get_key(const Type& a_) { return a_; }
    };

    template<class Type, class Traits = find_key_array_traits<Type>>
    class find_key_array: public auto_array<Type>
    {
        using base = auto_array<Type>;
        using key_type = typename Traits::key_type;

    public:
        void delete_at(uint32_t index_) { base::erase(index_, 1); }

        void delete_at(uint32_t index_, uint32_t count_) { base::erase(index_, count_); }

        bool delete_by_key(key_type key_)
        {
            const int index = find_by_key(key_);
            if (index < 0)
            { return false; }
            delete_at(index);
            return true;
        }

        void delete_all_by_key(key_type key_)
        {
            base::erase(
                    std::remove_if(
                            base::begin(), base::end(), [key_](const Type& el) {
                                return Traits::equals(Traits::get_key(el), key_);
                            }
                    ),
                    base::end());
        }

        std::optional<uint32_t> find(const Type& elem_) const
        {
            return find_by_key(Traits::get_key(elem_));
        }

        std::optional<uint32_t> find_by_key(key_type key_) const
        {
            for (int i = 0; i < base::Size(); i++)
            {
                if (Traits::equals(Traits::get_key(base::get(i)), key_))
                { return i; }
            }
            return {};
        }

        std::optional<uint32_t> push_back_unique(const Type& src_)
        {
            if (find(src_))
            { return -1; }
            return this->push_back(src_);
        }

        uint32_t find_or_push_back(const Type& src_)
        {
            auto index = find(src_);
            if (index)
            { return index; }
            return this->push_back(src_);
        }
    };

    template<class Type>
    struct reference_array_find_key_array_traits
    {
        using key_type = const Type&;

        static bool equals(key_type a_, key_type b_) { return a_ == b_; }

        static key_type get_key(const Type& a_) { return a_.get(); }
    };

    template<class Type>
    class reference_array:
            public find_key_array<mafia::game_types::Ref<Type>, reference_array_find_key_array_traits<Type>>
    {
        using base = find_key_array<mafia::game_types::Ref<Type>, reference_array_find_key_array_traits<Type>>;
        using traits = reference_array_find_key_array_traits<Type>;

    public:
        using base::delete_at;

        bool delete_at(const Type* el_) { return base::delete_by_key(el_); }

        bool delete_at(const mafia::game_types::Ref<Type>& src_) const
        {
            return base::delete_by_key(
                    traits::get_key(src_));
        }

        bool find(const Type* el_) { return base::find_by_key(el_); }

        bool find(const mafia::game_types::Ref<Type>& src_) const { return base::find_by_key(traits::get_key(src_)); }
    };

    /*
    static unsigned int rv_map_hash_string_case_sensitive(const char* key, int hashValue = 0) noexcept {
        while (*key) {
            hashValue = hashValue * 33 + static_cast<unsigned char>(*key++);
        }
        return hashValue;
    }
    static unsigned int rv_map_hash_string_case_insensitive(const char* key, int hashValue = 0) noexcept {
        while (*key) {
            hashValue = hashValue * 33 + static_cast<unsigned char>(tolower(*key++));
        }
        return hashValue;
    }
    */

    static unsigned int rv_map_hash_string_case_sensitive(std::string_view key, int hashValue = 0) noexcept
    {
        for (auto& it : key)
        {
            hashValue = hashValue * 33 + static_cast<unsigned char>(it);
        }
        return hashValue;
    }

    static unsigned int rv_map_hash_string_case_insensitive(std::string_view key, int hashValue = 0) noexcept
    {
        for (auto& it : key)
        {
            hashValue = hashValue * 33 + static_cast<unsigned char>(tolower(it));
        }
        return hashValue;
    }

    struct map_string_to_class_trait
    {
        /*
        static unsigned int hash_key(const char* key) noexcept {
            return rv_map_hash_string_case_sensitive(key);
        }
        static bool compare_keys(const char* k1, const char* k2) noexcept {
            return strcmp(k1, k2) == 0;
        }
        */
        static unsigned int hash_key(std::string_view key) noexcept
        {
            return rv_map_hash_string_case_sensitive(key);
        }

        static bool compare_keys(std::string_view k1, std::string_view k2) noexcept
        {
            if (k1.length() != k2.length())
            { return false; }
            return k1.compare(k2) == 0;
        }
    };

    struct map_string_to_class_trait_caseinsensitive: map_string_to_class_trait
    {
        /*
        static unsigned int hash_key(const char* key) noexcept {
            return rv_map_hash_string_case_insensitive(key);
        }

        static bool compare_keys(const char* k1, const char* k2) noexcept {
#ifdef __GNUC__
            return std::equal(k1, k1 + strlen(k1),
                              k2, [](unsigned char l, unsigned char r) { return l == r || tolower(l) == tolower(r); });
#else
            return _strcmpi(k1, k2) == 0;
#endif
        }
        */

        static unsigned int hash_key(std::string_view key) noexcept
        {
            return rv_map_hash_string_case_insensitive(key);
        }

        static bool compare_keys(std::string_view k1, std::string_view k2) noexcept
        {
            if (k1.length() != k2.length())
            { return false; }

            return std::equal(
                    k1.begin(), k1.end(),
                    k2.begin(), [](unsigned char l, unsigned char r) { return l == r || tolower(l) == tolower(r); }
            );
        }
    };

    template<class Type, class Container, class Traits = map_string_to_class_trait>
    class map_string_to_class
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
            const map_string_to_class<Type, Container, Traits>* _map;

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
            explicit iterator(const map_string_to_class<Type, Container, Traits>& base_)
            {
                _table = 0;
                _item = 0;
                _map = &base_;
                get_next();
            }

            iterator(const map_string_to_class<Type, Container, Traits>& base_, bool)
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

        map_string_to_class() {}

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
            auto newTable = rv_allocator<Container>::create_array(tableSize);
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
            rv_allocator<Container>::destroy_deallocate(newTable, oldTableCount);
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
                _table = rv_allocator<Container>::create_array(_tableCount);
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
    Type map_string_to_class<Type, Container, Traits>::_null_entry;

}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif //DEF_MAFIA_CORE_CONTAINERS_H
