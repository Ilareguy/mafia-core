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

#ifndef DEF_MAFIA_CORE_CONTAINERS_CompactArray_H
#define DEF_MAFIA_CORE_CONTAINERS_CompactArray_H

#include <algorithm>
#include "../game_types/rv_allocator.h"
#include "../ref_count.h"

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    template<class Type, class Allocator = mafia::game_types::RVAllocator<char>>  //Has to be allocator of type char
    class CompactArray: public mafia::RefCountBase
    {
        //static_assert(std::is_literal_type<Type>::value, "Type must be a literal type");
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
            using _Unchecked_type = pointer;

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

            difference_type operator-(const iterator& other) const { return p_ - other.p_; }

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
            using _Unchecked_type = pointer;

            const_iterator(): p_(nullptr) {}

            explicit const_iterator(const Type* p) noexcept: p_(p) {}

            const_iterator(const typename CompactArray<Type>::iterator& other): p_(other.p_) {}

            const_iterator(const const_iterator& other) noexcept: p_(other.p_) {}

            const_iterator& operator=(const const_iterator& other)
            {
                p_ = other.p_;
                return *this;
            }

            const_iterator& operator=(const typename CompactArray<Type>::iterator& other)
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
        static CompactArray* create(size_t number_of_elements_)
        {
            const size_t size = sizeof(CompactArray) + sizeof(Type) * (number_of_elements_ -
                                                                       1);  //-1 because we already have one element in CompactArray
            auto* buffer = reinterpret_cast<CompactArray*>(Allocator::allocate(size));
#pragma warning(suppress : 26409)  //don't use new/delete
            new(buffer) CompactArray(number_of_elements_);
            return buffer;
        }

        static CompactArray* create_zero(size_t number_of_elements_)
        {
            const size_t size = sizeof(CompactArray) + sizeof(Type) * (number_of_elements_ -
                                                                       1);  //-1 because we already have one element in CompactArray
            auto* buffer = reinterpret_cast<CompactArray*>(Allocator::allocate(size));
#pragma warning(suppress : 26409)  //don't use new/delete
            std::memset(buffer, 0, size);
            new(buffer) CompactArray(number_of_elements_);
            return buffer;
        }

        static CompactArray* create(const CompactArray& other)
        {
            const size_t sizeElements = other.size();
            const size_t sizeBytes = sizeof(CompactArray) + sizeof(Type) * (sizeElements - 1);
            auto* buffer = reinterpret_cast<CompactArray*>(Allocator::allocate(sizeBytes));
            new(buffer) CompactArray(sizeElements);
            std::copy(other.data(), other.data() + other.size(), buffer->data());
            return buffer;
        }

        template<class _InIt>
        static CompactArray* create(_InIt beg, _InIt end)
        {
            const size_t sizeElements = (std::distance(beg, end));
            const size_t sizeBytes = sizeof(CompactArray) + sizeof(Type) * (sizeElements - 1);
            auto* buffer = reinterpret_cast<CompactArray*>(Allocator::allocate(sizeBytes));
            std::memset(buffer, 0, sizeBytes);
#pragma warning(suppress : 26409)  //don't use new/delete
            new(buffer) CompactArray(sizeElements);

            size_t index = 0;
            for (auto& i = beg; i < end; ++i)
            {
                buffer->data()[index++] = *i;
            }

            return buffer;
        }

        //Specialty function to copy less elements or to allocate more space
        static CompactArray* create(const CompactArray& other, size_t element_count)
        {
            const size_t size = other.size();
            auto buffer = reinterpret_cast<CompactArray*>(Allocator::allocate(
                    sizeof(CompactArray) + sizeof(Type) * (element_count - 1)));
            new(buffer) CompactArray(element_count);
            std::memset(buffer->data(), 0, element_count * sizeof(Type));
            const auto elements_to_copy = std::min(size, element_count);

            std::copy(other.cbegin(), other.cbegin() + elements_to_copy, buffer->begin());

            return buffer;
        }

        void del() const
        {
            this->~CompactArray();
            const void* _thisptr = this;
#pragma warning(suppress : 26492)  //don't cast away const
            const auto _thisptr2 = const_cast<void*>(_thisptr);
            Allocator::deallocate(static_cast<char*>(_thisptr2), _size - 1 + sizeof(CompactArray));
        }

        size_t _size;
        Type _data {};

    private:
        explicit CompactArray(const size_t size_) noexcept
        {
            _size = size_;
        }
    };
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_CompactArray_H
