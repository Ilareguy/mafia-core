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

#ifndef DEF_MAFIA_CORE_CONTAINERS_AutoArray_H
#define DEF_MAFIA_CORE_CONTAINERS_AutoArray_H

#include "rv_array.h"
#include "../game_types/rv_allocator.h"
#include <stdexcept>

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    template<class Type, class Allocator = mafia::game_types::RVAllocator<Type>, size_t growthFactor = 32>
    class
#ifdef _MSC_VER
        __declspec(empty_bases)
#endif
        //#TODO try GCC maybe __attribute__((__packed__)) or #pragma pack. But be careful of the bool in local alloc
    AutoArray: public RVArray<Type>,
               private Allocator
    {
        typedef RVArray<Type> base;

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
                memmove((void*) newData, base::_data, base::_n * sizeof(Type));
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

        constexpr AutoArray() noexcept: RVArray<Type>(), _maxItems(0) {}

        AutoArray(const std::initializer_list<Type>& init_): RVArray<Type>(), _maxItems(0)
        {
            insert(end(), init_.begin(), init_.end());
        }

        template<class _InIt>
        AutoArray(_InIt first_, _InIt last_) : RVArray<Type>(), _maxItems(0)
        {
            insert(end(), first_, last_);
        }

        AutoArray(const AutoArray<Type>& copy_): RVArray<Type>(), _maxItems(0)
        {
            insert(end(), copy_.begin(), copy_.end());
        }

        AutoArray(AutoArray<Type>&& move_) noexcept: RVArray<Type>(std::move(move_)), _maxItems(move_._maxItems)
        {
            move_._maxItems = 0;
        }

        ~AutoArray()
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

        AutoArray& operator=(AutoArray&& move_) noexcept
        {
            base::_n = move_._n;
            _maxItems = move_._maxItems;
            base::_data = move_._data;
            move_._data = nullptr;
            move_._n = 0;
            move_._maxItems = 0;
            return *this;
        }

        AutoArray& operator=(const AutoArray& copy_)
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
                Allocator::deallocate(RVArray<Type>::_data);
                base::_n = 0;
                RVArray<Type>::_data = nullptr;
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
                Allocator::deallocate(RVArray<Type>::_data);
            }
            base::_data = nullptr;
            base::_n = 0;
            _maxItems = 0;
        }

        bool operator==(RVArray<Type> other_)
        {
            if (other_._n != base::_n ||
                ((base::_data == nullptr || other_._data == nullptr) && base::_data != other_._data))
            {
                return false;
            }
            auto index = 0;
            for (auto& it: other_)
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
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_AutoArray_H
