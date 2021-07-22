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

#ifndef DEF_MAFIA_CORE_CONTAINERS_STACK_ARRAY_H
#define DEF_MAFIA_CORE_CONTAINERS_STACK_ARRAY_H

#include "auto_array.h"

#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max

namespace mafia::containers
{
    template<class Type, class Allocator = mafia::game_types::RVAllocator<Type>>
    class StackArray: public AutoArray<Type, Allocator>
    {
        using base = AutoArray<Type, Allocator>;

    public:
        void push(const Type& src) { base::push_back(src); }

        const Type& top() const { return *(base::end() - 1); }

        void pop() { base::erase(base::end() - 1); }
    };
}

#pragma pop_macro("min")
#pragma pop_macro("max")

#endif // DEF_MAFIA_CORE_CONTAINERS_STACK_ARRAY_H
