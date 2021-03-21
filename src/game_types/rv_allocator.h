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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_RV_ALLOCATOR_H
#define DEF_MAFIA_CORE_GAME_TYPES_RV_ALLOCATOR_H

#include <type_traits>
#include <memory>

namespace mafia::game_types
{
    template<class Type>
    class RVAllocator
    {
    public:
        using value_type = Type;

        using pointer = value_type*;
        using const_pointer = const value_type*;

        using reference = value_type&;
        using const_reference = const value_type&;

        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        template<class Other>
        using rebind = RVAllocator<Other>;

        template<class Other>
        using rebind_alloc = RVAllocator<Other>;

        template<class Other>
        using rebind_traits = std::allocator_traits<RVAllocator<Other>>;

        RVAllocator() = default;

        template<class Other>
        explicit RVAllocator(const RVAllocator<Other>&) {}

        template<class Other>
        explicit RVAllocator(RVAllocator<Other>&&) {}

        static void deallocate(Type* ptr, size_t = 0)
        {
            return _internal::rv_allocator_deallocate_generic(ptr);
        }

        static void destroy(Type* ptr)
        {
            ptr->~Type();
        }

        static void destroy(Type* ptr, size_t size_)
        {
            for (size_t i = 0; i < size_; ++i)
            {
#pragma warning(suppress : 26409)
                (ptr + i)->~Type();
            }
        }

        static void destroy_deallocate(Type* ptr)
        {
            destroy(ptr);
            return _internal::rv_allocator_deallocate_generic(ptr);
        }

        static void destroy_deallocate(Type* _Ptr, size_t size_)
        {
            destroy(_Ptr, size_);
            return _internal::rv_allocator_deallocate_generic(_Ptr);
        }

        //This only allocates the memory! This will not be initialized to 0 and the allocated object will not have it's constructor called!
        //use the create* Methods instead
        static Type* allocate(const size_t count_)
        {
            return reinterpret_cast<Type*>(_internal::rv_allocator_allocate_generic(sizeof(Type) * count_));
        }

        //Allocates and Initializes one Object
        template<class... Types>
        static Type* create_single(Types&& ... args)
        {
            auto ptr = allocate(1);
            ::new(ptr) Type(std::forward<Types>(args)...);
            //Have to do a little more unfancy stuff for people that want to overload the new operator
            return ptr;
        }

        //Allocates and initializes array of Elements using the default constructor.
        static Type* create_array(const size_t count_)
        {
            if (count_ == 1)
            { return create_single(); }

            auto ptr = allocate(count_);

            for (size_t i = 0; i < count_; ++i)
            {
#pragma warning(suppress : 26409)
                ::new(ptr + i) Type();
            }

            return ptr;
        }

        static Type* create_uninitialized_array(const size_t count_)
        {
            if (count_ == 1)
            { return create_single(); }

            auto ptr = allocate(count_);

            return ptr;
        }

        static Type* reallocate(Type* ptr_, const size_t count_)
        {
            return reinterpret_cast<Type*>(_internal::rv_allocator_reallocate_generic(ptr_, sizeof(Type) * count_));
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_RV_ALLOCATOR_H
