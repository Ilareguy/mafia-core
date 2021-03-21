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

#include "containers.h"
#include "types.h"
#include "memory_utility.h"
#include "allocator_info.h"

namespace mafia
{
    class MemTableFunctions
    {
        //We don't want to expose this in the header. It's only used here
    public:
        virtual void* New(size_t size) = 0;
        virtual void* New(size_t size, const char* file, int line) = 0;
        virtual void Delete(void* mem) = 0;
        virtual void Delete(void* mem, const char* file, int line) = 0;
        virtual void* Realloc(void* mem, size_t size) = 0;
        virtual void* Realloc(void* mem, size_t size, const char* file, int line) = 0;
        virtual void* Resize(void* mem, size_t size) = 0; //This actually doesn't do anything.
        virtual void* NewPage(size_t size, size_t align) = 0;
        virtual void DeletePage(void* page, size_t size) = 0;
        virtual void* HeapAlloc(void* mem, size_t size) = 0;
        virtual void* HeapAlloc(void* mem, size_t size, const char* file, int line) = 0;//HeapAlloc
        virtual void* HeapDelete(void* mem, size_t size) = 0;
        virtual void* HeapDelete(void* mem, size_t size, const char* file, int line) = 0;//HeapFree
        virtual int something(void* mem, size_t unknown) = 0;
        virtual size_t GetPageRecommendedSize() = 0;
        virtual void* HeapBase() = 0;
        virtual size_t HeapUsed() = 0;
        virtual size_t HeapUsedByNew() = 0;
        virtual size_t HeapCommited() = 0;
        virtual int FreeBlocks() = 0;
        virtual int MemoryAllocatedBlocks() = 0;
        virtual void Report() = 0;
        virtual bool CheckIntegrity() = 0;
        virtual bool IsOutOfMemory() = 0;
        virtual void CleanUp() = 0;
        virtual void Lock() = 0;
        virtual void Unlock() = 0;
        const char* arr[6] {
                "tbb4malloc_bi", "tbb3malloc_bi", "jemalloc_bi", "tcmalloc_bi", "nedmalloc_bi", "custommalloc_bi"
        };
    };

    void mafia::set_game_value_vtable(uintptr_t vtable)
    {
        game_value::__vptr_def = vtable;
    }

    void* _internal::rv_allocator_allocate_generic(size_t _size)
    {
        static auto allocatorBase = mafia::memory_utility::get_allocator();
        auto* alloc = reinterpret_cast<MemTableFunctions*>(allocatorBase->genericAllocBase);
        return alloc->New(_size);
    }

    void _internal::rv_allocator_deallocate_generic(void* _Ptr)
    {
        static auto allocatorBase = mafia::memory_utility::get_allocator();
        auto* alloc = reinterpret_cast<MemTableFunctions*>(allocatorBase->genericAllocBase);
        alloc->Delete(_Ptr);
    }

    void* _internal::rv_allocator_reallocate_generic(void* _Ptr, size_t _size)
    {
        static auto allocatorBase = mafia::memory_utility::get_allocator();
        auto* alloc = reinterpret_cast<MemTableFunctions*>(allocatorBase->genericAllocBase);
        return alloc->Realloc(_Ptr, _size);
    }

    void* rv_pool_allocator::allocate(size_t count)
    {
        static auto allocatorBase = mafia::memory_utility::get_allocator();;
        typedef void* (__thiscall* allocFunc)(rv_pool_allocator*, size_t /*count*/);
        auto alloc = reinterpret_cast<allocFunc>(allocatorBase->poolFuncAlloc);
        auto allocation = alloc(this, count);
        return allocation;
    }

    void rv_pool_allocator::deallocate(void* data)
    {
        static auto allocatorBase = mafia::memory_utility::get_allocator();
        typedef void(__thiscall* deallocFunc)(rv_pool_allocator*, void* /*data*/);
        auto dealloc = reinterpret_cast<deallocFunc>(allocatorBase->poolFuncDealloc);
        dealloc(this, data);
    }
}
