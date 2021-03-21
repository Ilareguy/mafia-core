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

#ifndef DEF_MAFIA_CORE_CONTAINERS_FIND_KEY_ARRAY_H
#define DEF_MAFIA_CORE_CONTAINERS_FIND_KEY_ARRAY_H

#include "auto_array.h"
#include <optional>

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    template<class Type>
    struct FindKeyArrayTraits
    {
        using key_type = const Type&;

        static bool equals(key_type a_, key_type b_) { return a_ == b_; }

        static key_type get_key(const Type& a_) { return a_; }
    };

    template<class Type, class Traits = FindKeyArrayTraits<Type>>
    class FindKeyArray: public AutoArray<Type>
    {
        using base = AutoArray<Type>;
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
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_FIND_KEY_ARRAY_H
