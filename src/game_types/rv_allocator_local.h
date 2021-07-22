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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_RV_ALLOCATOR_LOCAL_H
#define DEF_MAFIA_CORE_GAME_TYPES_RV_ALLOCATOR_LOCAL_H

#include "rv_allocator.h"

namespace mafia::game_types
{
    template<class Type, int Count, class Fallback = RVAllocator<Type>>
    class RVAllocatorLocal: private Fallback
    {
        //buffer will be aligned by this
        using buffer_type = int;

        //Raw size of buffer in bytes
        static const size_t buffersize_bytes = (Count * sizeof(Type));
        //Raw size in count of buffer_type elements
        static const size_t buffersize_count = (buffersize_bytes + sizeof(buffer_type) - 1) / sizeof(buffer_type);
#ifndef __linux__
        int dummy {0};
#endif
        buffer_type buf[buffersize_count] {0};
        bool has_data {false};

    public:
        Type* allocate(size_t count_)
        {
            if (count_ <= Count && !has_data)
            {
                has_data = true;
                return reinterpret_cast<Type*>(buf);
            }
            return Fallback::allocate(count_);
        }

        Type* reallocate(Type* ptr_, size_t count_)
        {
            if (ptr_ == reinterpret_cast<Type*>(buf))
            {
                if (count_ <= Count)
                { return reinterpret_cast<Type*>(buf); }
                return nullptr;
            }
            return Fallback::reallocate(ptr_, count_);
        }

        void deallocate(Type* ptr_, size_t count_ = 1)
        {
            if (ptr_ != reinterpret_cast<Type*>(buf))
            {
                Fallback::deallocate(ptr_, count_);
            }
            else
            {
                has_data = false;
            }
        }

        //Allocates and Initializes one Object
        template<class... Types>
        Type* create_single(Types&& ... args)
        {
            auto ptr = allocate(1);
            ::new(ptr) Type(std::forward<Types>(args)...);
            //Have to do a little more unfancy stuff for people that want to overload the new operator
            return ptr;
        }

        Type* create_uninitialized_array(const size_t count_)
        {
            if (count_ == 1)
            { return create_single(); }

            auto ptr = allocate(count_);

            return ptr;
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_RV_ALLOCATOR_LOCAL_H
