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
 * File created by Anthony Ilareguy on 20/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "memory_utility.h"
#include "allocator_info.h"
#include <future>
#include <Windows.h>
#include <Psapi.h>

#pragma comment (lib, "Psapi.lib") // GetModuleInformation
#pragma comment (lib, "version.lib") // GetFileVersionInfoSize

namespace mafia::memory_utility::_private
{
    AllocatorInfo allocator {};

    uintptr_t find_in_memory(uintptr_t baseAddress, uintptr_t moduleSize, const char* pattern, size_t patternLength);
    uintptr_t find_in_memory_pattern(
            uintptr_t baseAddress,
            uintptr_t moduleSize,
            const char* pattern,
            const char* mask,
            uintptr_t offset = 0
    );
    const char* get_RTTI_name(uintptr_t vtable);
    uintptr_t find_game_state(uintptr_t stack_base);

    // Basically Windows' IsBadReadPtr(). See https://stackoverflow.com/questions/496034/most-efficient-replacement-for-isbadreadptr
    bool ptr_check(void* p);
}

uintptr_t mafia::memory_utility::_private::find_in_memory(
        uintptr_t baseAddress,
        uintptr_t moduleSize,
        const char* pattern,
        size_t patternLength
)
{
    for (uintptr_t i = 0; i < moduleSize - patternLength; i++)
    {
        bool found = true;
        for (uintptr_t j = 0; j < patternLength; j++)
        {
            found &= pattern[j] == *reinterpret_cast<char*>(baseAddress + i + j);
            if (!found)
            {
                break;
            }
        }

        if (found)
        {
            return baseAddress + i;
        }
    }
    return 0;
}

uintptr_t mafia::memory_utility::_private::find_in_memory_pattern(
        uintptr_t baseAddress, uintptr_t moduleSize, const char* pattern, const char* mask, uintptr_t offset
)
{
    const auto patternLength = static_cast<uintptr_t>(strlen(mask));

    for (uintptr_t i = 0; i < moduleSize - patternLength; i++)
    {
        bool found = true;
        for (uintptr_t j = 0; j < patternLength; j++)
        {
            found &= mask[j] == '?' || pattern[j] == *reinterpret_cast<char*>(baseAddress + i + j);
            if (!found)
            {
                break;
            }
        }

        if (found)
        {
            return baseAddress + i + offset;
        }
    }
    return static_cast<uintptr_t>(0x0u);
}

const char* mafia::memory_utility::_private::get_RTTI_name(uintptr_t vtable)
{
    class v1 {
        virtual void doStuff() {}
    };
    class v2 : public v1 {
        virtual void doStuff() {}
    };
    v2* v = (v2*) vtable;
    auto& typex = typeid(*v);
#ifdef __GNUC__
    auto test = typex.name();
#else
    auto test = typex.raw_name();
#endif
    return test;
}

mafia::AllocatorInfo* mafia::memory_utility::get_allocator()
{
    return &_private::allocator;
}

bool mafia::memory_utility::_private::ptr_check(void* p)
{
    MEMORY_BASIC_INFORMATION mbi = {nullptr};
    if (::VirtualQuery(p, &mbi, sizeof(mbi)))
    {
        DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                      PAGE_EXECUTE_WRITECOPY);
        bool b = !(mbi.Protect & mask);
        // check the page is not a guard page
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
        { b = true; }

        return b;
    }
    return true;
}

uintptr_t mafia::memory_utility::_private::find_game_state(uintptr_t stack_base)
{
    auto checkValid = [](uintptr_t base) -> bool {
        if (ptr_check(reinterpret_cast<void*>(base)))
        { return false; }
        struct size_check
        {
            uintptr_t p1;//typearray
            uintptr_t typeCount;
            uintptr_t typeCapacity;
            uintptr_t p2;//functions
            int f_tableCount {0};
            int f_count {0};
            uintptr_t p3;//operators
            int o_tableCount {0};
            int o_count {0};
            uintptr_t p4;//nulars
            int n_tableCount {0};
            int n_count {0};
        };
        auto size_check_type = reinterpret_cast<size_check*>(base);
        if (size_check_type->typeCount != size_check_type->typeCapacity)
        {
            return false;
        } //auto_array size vs capacity. Should be compacted here.
        //Check if all the function tables are valid
        if (ptr_check(reinterpret_cast<void*>(size_check_type->p1))) return false;
        if (ptr_check(reinterpret_cast<void*>(size_check_type->p2))) return false;
        if (ptr_check(reinterpret_cast<void*>(size_check_type->p3))) return false;
        if (ptr_check(reinterpret_cast<void*>(size_check_type->p4))) return false;
        return true;
    };

    for (uintptr_t i = 0; i < 0x300; i += sizeof(uintptr_t))
    {
        const bool is_valid = checkValid(*reinterpret_cast<uintptr_t*>(stack_base + i));
        if (is_valid)
        { return *reinterpret_cast<uintptr_t*>(stack_base + i); }
    }

    return 0x0;
}

// Basically equivalent to `loader::do_function_walk()` in
// https://github.com/intercept/intercept/blob/master/src/host/loader/loader.cpp
void mafia::memory_utility::init(uintptr_t stack_base)
{
    auto game_state_ptr = reinterpret_cast<mafia::game_types::GameState*>(_private::find_game_state(stack_base));

    MODULEINFO modInfo = {nullptr};
    HMODULE hModule = GetModuleHandleA(nullptr);
    GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    const auto baseAddress = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
    const auto moduleSize = static_cast<uintptr_t>(modInfo.SizeOfImage);

    // Start them async before doing the other stuff so they are done when we are done parsing the script functions
    auto future_stringOffset = std::async(
            [&]() { return _private::find_in_memory(baseAddress, moduleSize, "tbb4malloc_bi", 13); }
    );

    // Second part of finding the allocator. Done here so the second memorySearch is done when we are done parsing
    // the Nulars
    auto future_allocatorVtablePtr = std::async(
            std::launch::deferred, [&]() {
                uintptr_t stringOffset = future_stringOffset.get();
                uintptr_t vtableStart = stringOffset - (0x09D20C70 - 0x09D20BE8);
                return vtableStart;
                //return (findInMemory(reinterpret_cast<char*>(&vtableStart), 4));
            }
    );

#if _WIN64 || __X86_64__
    auto future_poolFuncAlloc = std::async(
            [&]() {
                return _private::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x40\x53\x48\x83\xEC\x20\xFF\x41\x60\x48\x8B\x41\x08\x48\x8B\xD9\x48\x3B\xC1\x74\x0B\x48\x85\xC0\x74\x06\x48\x83\xC0\xE0\x75\x2B\x48\x8D\x41\x18\x48\x8B\x49\x20\x48\x3B\xC8\x74\x0E\x48\x85\xC9\x74\x09\x48\x8D\x41\xE0\x48\x85\xC0\x75\x10\x48\x8B\xCB\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x4C\x8B\x43\x08\x32\xC9\x45\x33\xD2\x4C\x3B\xC3\x74\x0B\x4D\x85\xC0\x74\x06\x49\x83\xC0\xE0\x75\x2A\x4C\x8B\x43\x20\x48\x8D\x43\x18\x4C\x3B\xC0",
                        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                );
            }
    );

    auto future_poolFuncDealloc = std::async(
            [&]() {
                return _private::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x48\x85\xD2\x0F\x84\x00\x00\x00\x00\x53\x48\x83\xEC\x20\x48\x63\x41\x58\x48\x89\x7C\x24\x00\x48\x8B\xFA\x48\xFF\xC8\x48\x8B\xD9\x48\x23\xC2\x48\x2B\xF8\x83\x3F\x00\x74\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x44\x8B\x07\x48\x8D\x0D\x00\x00\x00\x00\x48\x8B\xD7\x48\x8B\x7C\x24\x00\x48\x83\xC4\x20\x5B\xE9\x00\x00\x00\x00\x48\x8B\x47\x18\x48\x89\x02\x48\x83\x7F\x00\x00\x48\x89\x57\x18\x0F\x94\xC0\x48\x89\x7A\x08\xFF\x4F\x10\x41\x0F\x94\xC0\x84\xC0\x74\x46\x48\x8B\x4F\x28\x48\x8B\x47\x20\x48\x8D\x57\x20\x48\x89\x01\x48\x8B\x42\x08\x48\x8B\x0A",
                        "xxxxx????xxxxxxxxxxxxx?xxxxxxxxxxxxxxxxxxxxxxx????x????xxxxxx????xxxxxxx?xxxxxx????xxxxxxxxxx??xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                );
            }
    );
    //auto future_fileBanks = std::async([&]() {
    //    auto patternFindLoc = findInMemoryPattern("\x48\x83\xEC\x28\x48\x8B\x0D\x00\x00\x00\x00\xBA\x00\x00\x00\x00\x48\x8B\x01\xFF\x50\x08\x33\xC9\x48\x85\xC0\x74\x18\x48\x89\x08\x89\x48\x10\x89\x48\x08\x48\x89\x48\x28\x89\x48\x38\x89\x48\x30\x88\x48\x21\xEB\x03\x48\x8B\xC1\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9", "xxxxxxx????x????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????xxxxx");
    //    auto offs = *reinterpret_cast<uint32_t*>(patternFindLoc + 0x42);
    //    return patternFindLoc + 0x46 + offs;
    //});
#else
#endif

#if _WIN32 && !_WIN64
    //via profile context "scrpt"
        auto future_evaluateScript = std::async([&]() {
            //profiling branch
            auto patternFindLoc = _private::find_in_memory_pattern(
                    baseAddress, moduleSize,
                "\xA1\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x55\x56\x57\x8B\xF9\xA8\x01\x75\x1C\x83\xC8\x01\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x75\x49\xE8\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xC6\x05\x00\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB6\xC0\x8D\x4C\x24\x1C\x50\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xB4\x24\x00\x00\x00\x00\x83\xCB\xFF\x80\x3D\x00\x00\x00\x00\x00\xBD\x00\x00\x00\x00\x74\x68\x51\x8B\x0E\x8B\xD4\x85\xC9\x74\x06\x8B\xC5\xF0\x0F\xC1\x01\x8D\x44\x24\x18\x89\x0A\x50",
                "x????xx????xxxxxxxxxxxxxxx????????x????xx????????xx?????xxx????xx????x????x????xx????????xx????????xx????????x????x????x????xx?????x????x????xxxxxxxxx????x????xxx????xxxxx?????x????xxxxxxxxxxxxxxxxxxxxxxxx");

            if (!patternFindLoc) //non-prof
                patternFindLoc = _private::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x8b\x44\x24\x00\x81\xec\x00\x00\x00\x00\x8b\x00\x53\x55\x56\x57\x8b\xf9\x85\xc0\x74\x00\x80\x78\x00\x00\x75\x00\x8b\x8c\x24\x00\x00\x00\x00\x8b\x09\x85\xc9\x0f\x84\x00\x00\x00\x00\x83\x79\x00\x00\x0f\x84\x00\x00\x00\x00\x83\xbc\x24\x00\x00\x00\x00\x00\x75\x00\x80\xbf\x00\x00\x00\x00\x00\x74\x00\xc7\x84\x24\x00\x00\x00\x00\x00\x00\x00\x00\xeb\x00\x85\xc0\x74\x00\x83\xc0\x00\xeb\x00\xb8\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x83\xc4\x00\x8b\x87\x00\x00\x00\x00\x83\xcd\x00\xc7\x40\x00\x00\x00\x00\x00\x8d\x44\x24\x00\x8b\xb7\x00\x00\x00\x00\x83\xc6\x00\x3b\xf0\x74\x00\x8b\x16\x85\xd2\x74\x00\x8b\xc5\xf0\x0f\xc1\x02\x48\x75\x00\x8b\x0d\x00\x00\x00\x00\x52\x8b\x01\xff\x50\x00\xc7\x06\x00\x00\x00\x00\x6a\x00\x8d\x4c\x24\x00\xe8\x00\x00\x00\x00\x8b\xb7\x00\x00\x00\x00\x8b\xce\xe8\x00\x00\x00\x00\x8b\xce\x8a\xd8\xe8\x00\x00\x00\x00\x8b\x94\x24\x00\x00\x00\x00\x89\x84\x24\x00\x00\x00\x00\x35\x00\x00\x00\x00\x8b\xc8\x88\x9c\x24\x00\x00\x00\x00\xd1\xf9\xbb\x00\x00\x00\x00\x2b\xc1\x89\x8c\x24",
                        "xxx?xx????xxxxxxxxxxx?xx??x?xxx????xxxxxx????xx??xx????xxx?????x?xx?????x?xxx????????x?xxx?xx?x?x????xx????x????xx?xx????xx?xx?????xxx?xx????xx?xxx?xxxxx?xxxxxxxx?xx????xxxxx?xx????x?xxx?x????xx????xxx????xxxxx????xxx????xxx????x????xxxxx????xxx????xxxxx");

            return patternFindLoc;
        });
        //Checked on prof/non-prof
        auto future_varSetLocal = std::async([&]() {
            auto patternFindLoc = _private::find_in_memory_pattern(
                    baseAddress, moduleSize,
                "\x51\x56\x57\x8B\x7C\x24\x10\x8B\xF1\x85\xFF\x0F\x84\x00\x00\x00\x00\x80\x3F\x00\x0F\x84\x00\x00\x00\x00\x57\xC7\x44\x24\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x54\x24\x14\x83\xC4\x04\x85\xC0\x74\x09\xB9\x00\x00\x00\x00\xF0\x0F\xC1\x08\x53\x83\xCB\xFF\x89\x44\x24\x14\x85\xD2\x74\x15\x8B\xC3\xF0\x0F\xC1\x02\x48\x75\x0C\x8B\x0D\x00\x00\x00\x00\x52\x8B\x01\xFF\x50\x0C\x8D\x4C\x24\x14\xE8\x00\x00\x00\x00\x80\x7C\x24\x00\x00\x75\x12\x80\x3F\x5F\x74\x0D\x6A\x05\x56\xE8\x00\x00\x00\x00\x83\xC4\x08\xEB\x56",
                "xxxxxxxxxxxxx????xxxxx????xxxx?????x????xxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????xxx??xxxxxxxxxxx????xxxxx");

            return patternFindLoc;
        });
#elif _WIN32 && _WIN64

    //via profile context "scrpt"
    auto future_evaluateScript = std::async(
            [&]() {
                //profiling branch missing for x64
                auto patternFindLoc = _private::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x48\x89\x5C\x24\x00\x4C\x89\x4C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x83\xCE\xFF\x49\x8B\xD9\x4D\x8B\xE8\x4C\x8B\xF2\x4C\x8B\xF9\xA8\x01\x75\x1A\x83\xC8\x01\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x89\x35\x00\x00\x00\x00\x89\x05\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x4C\x8D\x25\x00\x00\x00\x00\x75\x46\xE8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\x49\x8B\xCC\x44\x8B\xC8\x4C\x89\x25\x00\x00\x00\x00\x48\x89\x15\x00\x00\x00\x00\x4C\x89\x05\x00\x00\x00\x00\x89\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xC6\x05\x00\x00\x00\x00\x00\x89\x05\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x48\x89\x7C\x24\x00\x33\xFF\x83\x3D\x00\x00\x00\x00\x00\x48\x89\x7C\x24\x00\x88\x44\x24\x40\x74\x45\x40\x38\x3D\x00\x00\x00\x00\x75\x12\x84\xC0\x74\x38\xFF\x15\x00\x00\x00\x00\x39\x05\x00\x00\x00\x00\x75\x2A",
                        "xxxx?xxxx?xxxxxxxxxxxxxxx????xxx????xx????xxxxxxxxxxxxxxxxxxxxxxxxx????????xx????xx????xx?????xxx????xxx????xxx????xxx????xxxxxxxxx????xxx????xxx????xx????x????xx?????xx????xxx????xxxx????xxxx?xxxx?????xxxx?xxxxxxxxx????xxxxxxxx????xx????xx"
                );

                if (!patternFindLoc)
                { //non-prof
                    patternFindLoc = _private::find_in_memory_pattern(
                            baseAddress, moduleSize,
                            "\x4C\x89\x4C\x24\x00\x4C\x89\x44\x24\x00\x55\x56\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\x00\x48\x8B\xF2\x4C\x8B\xF1\x48\x85\xC0\x74\x06\x80\x78\x10\x00\x75\x16\x49\x8B\x09\x48\x85\xC9\x0F\x84\x00\x00\x00\x00\x83\x79\x08\x00\x0F\x84\x00\x00\x00\x00\x48\x89\x9C\x24\x00\x00\x00\x00\x48\x89\xBC\x24\x00\x00\x00\x00\x4C\x89\xA4\x24\x00\x00\x00\x00\x4C\x89\xAC\x24\x00\x00\x00\x00\x4C\x8B\xAD\x00\x00\x00\x00\x4D\x85\xED\x75\x2E\x45\x38\xAE\x00\x00\x00\x00\x74\x09\x4C\x8D\x2D\x00\x00\x00\x00\xEB\x1C\x48\x8D\x50\x10\x48\x85\xC0\x75\x07\x48\x8D\x15\x00\x00\x00\x00",
                            "xxxx?xxxx?xxxxxxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxx????xxxx????xxxx????xxxx????xxxx????xxx????xxxxxxxx????xxxxx????xxxxxxxxxxxxxx????"
                    );
                }

                return patternFindLoc;
            }
    );

    //Checked on prof/non-prof
    auto future_varSetLocal = std::async(
            [&]() {
                auto patternFindLoc = _private::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x30\x41\x0F\xB6\xE9\x4D\x8B\xF0\x48\x8B\xF2\x48\x8B\xF9\x48\x85\xD2\x0F\x84\x00\x00\x00\x00\x80\x3A\x00\x0F\x84\x00\x00\x00\x00\x48\x8B\xCA\x48\x89\x5C\x24\x00\x48\xC7\x44\x24\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x54\x24\x00\x48\x85\xC0\x74\x03\xF0\xFF\x00\x48\x89\x44\x24\x00\x48\x85\xD2\x74\x17\xF0\xFF\x0A\x75\x0D\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\x01\xFF\x50\x18\x48\x8B\x44\x24\x00\x48\x85\xC0\x74\x6E\x48\x8D\x48\x10\xE8\x00\x00\x00\x00\x84\xC0\x48\x8B\x44\x24\x00\x75\x5C\x48\x85\xC0\x74\x57\x8B\x08\x83\xF9\x01\x7E\x37\x48\x8B\x4C\x24\x00\x48\x83\xC1\x10\xE8\x00\x00\x00\x00\x48\x8B\x54\x24\x00\x48\x85\xC0\x74\x03\xF0\xFF\x00\x48\x89\x44\x24\x00\x48\x85\xD2\x74\x17\xF0\xFF\x0A\x75\x0D\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\x01\xFF\x50\x18\x48\x8B\x44\x24\x00\x48\x85\xC0\x74\x0F\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\x44\x24\x00\x80\x7C\x24\x00\x00\x75\x14\x80\x3E\x5F\x74\x0F\xBA\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\xEB\x58",
                        "xxxx?xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxx????xxxxxxx?xxxx?????x????xxxx?xxxxxxxxxxxx?xxxxxxxxxxxxx????xxxxxxxxxx?xxxxxxxxxx????xxxxxx?xxxxxxxxxxxxxxxxxx?xxxxx????xxxx?xxxxxxxxxxxx?xxxxxxxxxxxxx????xxxxxxxxxx?xxxxxxxxx?x????xxxx?xxx??xxxxxxxx????xxxx????xx"
                );

                return patternFindLoc;
            }
    );
#endif

    const uintptr_t allocatorVtablePtr = future_allocatorVtablePtr.get();
    const char* test = _private::get_RTTI_name(allocatorVtablePtr);
    // assert(strcmp(test, ".?AVMemTableFunctions@@") == 0);
    _private::allocator.genericAllocBase = allocatorVtablePtr;
}
