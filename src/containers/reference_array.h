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

#ifndef DEF_MAFIA_CORE_CONTAINERS_REFERENCE_ARRAY_H
#define DEF_MAFIA_CORE_CONTAINERS_REFERENCE_ARRAY_H

#include "find_key_array.h"
#include "../ref.h"

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    template<class Type>
    struct ReferenceArrayFindKeyArrayTraits
    {
        using key_type = const Type&;

        static bool equals(key_type a_, key_type b_) { return a_ == b_; }

        static key_type get_key(const Type& a_) { return a_.get(); }
    };

    template<class Type>
    class ReferenceArray: public FindKeyArray<mafia::Ref<Type>, ReferenceArrayFindKeyArrayTraits<Type>>
    {
        using base = FindKeyArray<mafia::Ref<Type>, ReferenceArrayFindKeyArrayTraits<Type>>;
        using traits = ReferenceArrayFindKeyArrayTraits<Type>;

    public:
        using base::delete_at;

        bool delete_at(const Type* el_) { return base::delete_by_key(el_); }

        bool delete_at(const mafia::Ref<Type>& src_) const
        {
            return base::delete_by_key(
                    traits::get_key(src_));
        }

        bool find(const Type* el_) { return base::find_by_key(el_); }

        bool find(const mafia::Ref<Type>& src_) const { return base::find_by_key(traits::get_key(src_)); }
    };
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_REFERENCE_ARRAY_H
