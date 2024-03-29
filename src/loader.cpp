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
 * File created by Anthony Ilareguy on 24/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "loader.h"
#include "mafia.h"
#include "rv_controller.h"
#include "memory_utility.h"
#include "sqf_functions.h"
#include "logging.h"
#include "game_types/game_data/code.h"
#include <future>
#include <Windows.h>
#include <Psapi.h>
#include <cassert>

#pragma comment (lib, "Psapi.lib")
#pragma comment (lib, "version.lib")

using namespace mafia;
using namespace mafia::game_types;
using namespace std::literals::string_view_literals;

Loader::Loader(): _attached(false), _patched(false) {}

Loader::~Loader() = default;

bool Loader::get_function(std::string_view function_name_, UnaryFunction& function_)
{
    auto it = _unary_operators.find(function_name_);
    if (it != _unary_operators.end())
    {
        function_ = reinterpret_cast<UnaryFunction>(it->second[0].op->procedure_addr);
        return true;
    }
    return false;
}

bool Loader::get_function(std::string_view function_name_, UnaryFunction& function_, std::string_view arg_signature_)
{
    auto it = _unary_operators.find(function_name_);
    if (it != _unary_operators.end())
    {
        for (auto op : it->second)
        {
            if (op.op->arg_type.type().count(arg_signature_))
            {
                function_ = reinterpret_cast<UnaryFunction>(op.op->procedure_addr);
                return true;
            }
        }
        return false;
    }
    return false;
}

bool Loader::get_function(std::string_view function_name_, BinaryFunction& function_)
{
    auto it = _binary_operators.find(function_name_);
    if (it != _binary_operators.end())
    {
        function_ = reinterpret_cast<BinaryFunction>(it->second[0].op->procedure_addr);
        return true;
    }
    return false;
}

bool Loader::get_function(
        std::string_view function_name_,
        BinaryFunction& function_,
        std::string_view arg1_signature_,
        std::string_view arg2_signature_
)
{
    auto it = _binary_operators.find(function_name_);
    if (it != _binary_operators.end())
    {
        for (auto op : it->second)
        {
            if (op.op->arg1_type.type().count(arg1_signature_) && op.op->arg2_type.type().count(arg2_signature_))
            {
                function_ = reinterpret_cast<BinaryFunction>(op.op->procedure_addr);
                return true;
            }
        }
        return false;
    }
    return false;
}

bool Loader::get_function(std::string_view function_name_, NularFunction& function_)
{
    auto it = _nular_operators.find(function_name_);
    if (it != _nular_operators.end())
    {
        function_ = reinterpret_cast<NularFunction>(it->second[0].op->procedure_addr);
        return true;
    }
    return false;
}

const UnaryMap_t& Loader::unary() const
{
    return _unary_operators;
}

const BinaryMap_t& Loader::binary() const
{
    return _binary_operators;
}

const NularMap_t& Loader::nular() const
{
    return _nular_operators;
}

const AllocatorInfo* Loader::get_allocator() const
{
    return &_allocator;
}

const SQFRegisterFunctions& Loader::get_register_sqf_info() const
{
    return _sqf_register_funcs;
}

void Loader::init(uintptr_t stack_base)
{
    const auto game_state_addr = find_game_state(stack_base);
    _game_state_ptr = reinterpret_cast<mafia::game_types::GameState*>(game_state_addr);

    MODULEINFO modInfo = {nullptr};
    HMODULE hModule = GetModuleHandleA(nullptr);
    GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    const auto baseAddress = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
    const auto moduleSize = static_cast<uintptr_t>(modInfo.SizeOfImage);

    // Start them async before doing the other stuff so they are done when we are done parsing the script functions
    auto future_stringOffset = std::async(
            [&]() { return memory_utility::find_in_memory(baseAddress, moduleSize, "tbb4malloc_bi", 13); }
    );

    // Second part of finding the allocator. Done here so the second memorySearch is done when we are done parsing
    // the Nulars
    auto future_allocatorVtablePtr = std::async(
            std::launch::deferred, [&]() {
                uintptr_t stringOffset = future_stringOffset.get();
                return (memory_utility::find_in_memory(
                        baseAddress,
                        moduleSize,
                        reinterpret_cast<char*>(&stringOffset),
                        sizeof(uintptr_t)) - sizeof(uintptr_t));
            }
    );

#if _WIN64 || __X86_64__
    auto future_poolFuncAlloc = std::async(
            [&]() {
                return memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x40\x53\x48\x83\xEC\x20\xFF\x41\x60\x48\x8B\x41\x08\x48\x8B\xD9\x48\x3B\xC1\x74\x0B\x48\x85\xC0\x74\x06\x48\x83\xC0\xE0\x75\x2B\x48\x8D\x41\x18\x48\x8B\x49\x20\x48\x3B\xC8\x74\x0E\x48\x85\xC9\x74\x09\x48\x8D\x41\xE0\x48\x85\xC0\x75\x10\x48\x8B\xCB\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x4C\x8B\x43\x08\x32\xC9\x45\x33\xD2\x4C\x3B\xC3\x74\x0B\x4D\x85\xC0\x74\x06\x49\x83\xC0\xE0\x75\x2A\x4C\x8B\x43\x20\x48\x8D\x43\x18\x4C\x3B\xC0",
                        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                );
            }
    );

    auto future_poolFuncDealloc = std::async(
            [&]() {
                return memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x48\x85\xD2\x0F\x84\x00\x00\x00\x00\x53\x48\x83\xEC\x20\x48\x63\x41\x58\x48\x89\x7C\x24\x00\x48\x8B\xFA\x48\xFF\xC8\x48\x8B\xD9\x48\x23\xC2\x48\x2B\xF8\x83\x3F\x00\x74\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x44\x8B\x07\x48\x8D\x0D\x00\x00\x00\x00\x48\x8B\xD7\x48\x8B\x7C\x24\x00\x48\x83\xC4\x20\x5B\xE9\x00\x00\x00\x00\x48\x8B\x47\x18\x48\x89\x02\x48\x83\x7F\x00\x00\x48\x89\x57\x18\x0F\x94\xC0\x48\x89\x7A\x08\xFF\x4F\x10\x41\x0F\x94\xC0\x84\xC0\x74\x46\x48\x8B\x4F\x28\x48\x8B\x47\x20\x48\x8D\x57\x20\x48\x89\x01\x48\x8B\x42\x08\x48\x8B\x0A",
                        "xxxxx????xxxxxxxxxxxxx?xxxxxxxxxxxxxxxxxxxxxxx????x????xxxxxx????xxxxxxx?xxxxxx????xxxxxxxxxx??xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                );
            }
    );
#else
    auto future_poolFuncAlloc = std::async(
            [&]() {
                return memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x56\x8B\xF1\xFF\x46\x38\x8B\x46\x04\x3B\xC6\x74\x09\x85\xC0\x74\x05\x83\xC0\xF0\x75\x26\x8B\x4E\x10\x8D\x46\x0C\x3B\xC8\x74\x0B\x85\xC9\x74\x07\x8D\x41\xF0\x85\xC0\x75\x11",
                        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                );
            }
    );
    auto future_poolFuncDealloc = std::async(
            [&]() {
                return memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x8B\x44\x24\x04\x85\xC0\x74\x09\x89\x44\x24\x04\xE9",
                        "xxxxxxxxxxxxx"
                );
            }
    );
#endif

#if _WIN32 && !_WIN64
    //via profile context "scrpt"
    auto future_evaluateScript = std::async(
            [&]() {
                //profiling branch
                auto patternFindLoc = memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\xA1\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x55\x56\x57\x8B\xF9\xA8\x01\x75\x1C\x83\xC8\x01\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x75\x49\xE8\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xC6\x05\x00\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB6\xC0\x8D\x4C\x24\x1C\x50\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xB4\x24\x00\x00\x00\x00\x83\xCB\xFF\x80\x3D\x00\x00\x00\x00\x00\xBD\x00\x00\x00\x00\x74\x68\x51\x8B\x0E\x8B\xD4\x85\xC9\x74\x06\x8B\xC5\xF0\x0F\xC1\x01\x8D\x44\x24\x18\x89\x0A\x50",
                        "x????xx????xxxxxxxxxxxxxxx????????x????xx????????xx?????xxx????xx????x????x????xx????????xx????????xx????????x????x????x????xx?????x????x????xxxxxxxxx????x????xxx????xxxxx?????x????xxxxxxxxxxxxxxxxxxxxxxxx"
                );

                if (!patternFindLoc)
                { //non-prof
                    patternFindLoc = memory_utility::find_in_memory_pattern(
                            baseAddress, moduleSize,
                            "\x8b\x44\x24\x00\x81\xec\x00\x00\x00\x00\x8b\x00\x53\x55\x56\x57\x8b\xf9\x85\xc0\x74\x00\x80\x78\x00\x00\x75\x00\x8b\x8c\x24\x00\x00\x00\x00\x8b\x09\x85\xc9\x0f\x84\x00\x00\x00\x00\x83\x79\x00\x00\x0f\x84\x00\x00\x00\x00\x83\xbc\x24\x00\x00\x00\x00\x00\x75\x00\x80\xbf\x00\x00\x00\x00\x00\x74\x00\xc7\x84\x24\x00\x00\x00\x00\x00\x00\x00\x00\xeb\x00\x85\xc0\x74\x00\x83\xc0\x00\xeb\x00\xb8\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x83\xc4\x00\x8b\x87\x00\x00\x00\x00\x83\xcd\x00\xc7\x40\x00\x00\x00\x00\x00\x8d\x44\x24\x00\x8b\xb7\x00\x00\x00\x00\x83\xc6\x00\x3b\xf0\x74\x00\x8b\x16\x85\xd2\x74\x00\x8b\xc5\xf0\x0f\xc1\x02\x48\x75\x00\x8b\x0d\x00\x00\x00\x00\x52\x8b\x01\xff\x50\x00\xc7\x06\x00\x00\x00\x00\x6a\x00\x8d\x4c\x24\x00\xe8\x00\x00\x00\x00\x8b\xb7\x00\x00\x00\x00\x8b\xce\xe8\x00\x00\x00\x00\x8b\xce\x8a\xd8\xe8\x00\x00\x00\x00\x8b\x94\x24\x00\x00\x00\x00\x89\x84\x24\x00\x00\x00\x00\x35\x00\x00\x00\x00\x8b\xc8\x88\x9c\x24\x00\x00\x00\x00\xd1\xf9\xbb\x00\x00\x00\x00\x2b\xc1\x89\x8c\x24",
                            "xxx?xx????xxxxxxxxxxx?xx??x?xxx????xxxxxx????xx??xx????xxx?????x?xx?????x?xxx????????x?xxx?xx?x?x????xx????x????xx?xx????xx?xx?????xxx?xx????xx?xxx?xxxxx?xxxxxxxx?xx????xxxxx?xx????x?xxx?x????xx????xxx????xxxxx????xxx????xxx????x????xxxxx????xxx????xxxxx"
                    );
                }

                return patternFindLoc;
            }
    );
    //Checked on prof/non-prof
    auto future_varSetLocal = std::async(
            [&]() {
                auto patternFindLoc = memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x51\x56\x57\x8B\x7C\x24\x10\x8B\xF1\x85\xFF\x0F\x84\x00\x00\x00\x00\x80\x3F\x00\x0F\x84\x00\x00\x00\x00\x57\xC7\x44\x24\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x54\x24\x14\x83\xC4\x04\x85\xC0\x74\x09\xB9\x00\x00\x00\x00\xF0\x0F\xC1\x08\x53\x83\xCB\xFF\x89\x44\x24\x14\x85\xD2\x74\x15\x8B\xC3\xF0\x0F\xC1\x02\x48\x75\x0C\x8B\x0D\x00\x00\x00\x00\x52\x8B\x01\xFF\x50\x0C\x8D\x4C\x24\x14\xE8\x00\x00\x00\x00\x80\x7C\x24\x00\x00\x75\x12\x80\x3F\x5F\x74\x0D\x6A\x05\x56\xE8\x00\x00\x00\x00\x83\xC4\x08\xEB\x56",
                        "xxxxxxxxxxxxx????xxxxx????xxxx?????x????xxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????xxx??xxxxxxxxxxx????xxxxx"
                );

                return patternFindLoc;
            }
    );
#elif _WIN32 && _WIN64
    //via profile context "scrpt"
    auto future_evaluateScript = std::async(
            [&]() {
                //profiling branch missing for x64
                auto patternFindLoc = memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x48\x89\x5C\x24\x00\x4C\x89\x4C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x83\xCE\xFF\x49\x8B\xD9\x4D\x8B\xE8\x4C\x8B\xF2\x4C\x8B\xF9\xA8\x01\x75\x1A\x83\xC8\x01\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\x89\x35\x00\x00\x00\x00\x89\x05\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x4C\x8D\x25\x00\x00\x00\x00\x75\x46\xE8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\x49\x8B\xCC\x44\x8B\xC8\x4C\x89\x25\x00\x00\x00\x00\x48\x89\x15\x00\x00\x00\x00\x4C\x89\x05\x00\x00\x00\x00\x89\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xC6\x05\x00\x00\x00\x00\x00\x89\x05\x00\x00\x00\x00\x48\x8D\x3D\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x48\x89\x7C\x24\x00\x33\xFF\x83\x3D\x00\x00\x00\x00\x00\x48\x89\x7C\x24\x00\x88\x44\x24\x40\x74\x45\x40\x38\x3D\x00\x00\x00\x00\x75\x12\x84\xC0\x74\x38\xFF\x15\x00\x00\x00\x00\x39\x05\x00\x00\x00\x00\x75\x2A",
                        "xxxx?xxxx?xxxxxxxxxxxxxxx????xxx????xx????xxxxxxxxxxxxxxxxxxxxxxxxx????????xx????xx????xx?????xxx????xxx????xxx????xxx????xxxxxxxxx????xxx????xxx????xx????x????xx?????xx????xxx????xxxx????xxxx?xxxx?????xxxx?xxxxxxxxx????xxxxxxxx????xx????xx"
                );

                if (!patternFindLoc)
                { //non-prof
                    patternFindLoc = memory_utility::find_in_memory_pattern(
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
                auto patternFindLoc = memory_utility::find_in_memory_pattern(
                        baseAddress, moduleSize,
                        "\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x30\x41\x0F\xB6\xE9\x4D\x8B\xF0\x48\x8B\xF2\x48\x8B\xF9\x48\x85\xD2\x0F\x84\x00\x00\x00\x00\x80\x3A\x00\x0F\x84\x00\x00\x00\x00\x48\x8B\xCA\x48\x89\x5C\x24\x00\x48\xC7\x44\x24\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x54\x24\x00\x48\x85\xC0\x74\x03\xF0\xFF\x00\x48\x89\x44\x24\x00\x48\x85\xD2\x74\x17\xF0\xFF\x0A\x75\x0D\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\x01\xFF\x50\x18\x48\x8B\x44\x24\x00\x48\x85\xC0\x74\x6E\x48\x8D\x48\x10\xE8\x00\x00\x00\x00\x84\xC0\x48\x8B\x44\x24\x00\x75\x5C\x48\x85\xC0\x74\x57\x8B\x08\x83\xF9\x01\x7E\x37\x48\x8B\x4C\x24\x00\x48\x83\xC1\x10\xE8\x00\x00\x00\x00\x48\x8B\x54\x24\x00\x48\x85\xC0\x74\x03\xF0\xFF\x00\x48\x89\x44\x24\x00\x48\x85\xD2\x74\x17\xF0\xFF\x0A\x75\x0D\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\x01\xFF\x50\x18\x48\x8B\x44\x24\x00\x48\x85\xC0\x74\x0F\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\x44\x24\x00\x80\x7C\x24\x00\x00\x75\x14\x80\x3E\x5F\x74\x0F\xBA\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\xEB\x58",
                        "xxxx?xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxx????xxxxxxx?xxxx?????x????xxxx?xxxxxxxxxxxx?xxxxxxxxxxxxx????xxxxxxxxxx?xxxxxxxxxx????xxxxxx?xxxxxxxxxxxxxxxxxx?xxxxx????xxxx?xxxxxxxxxxxx?xxxxxxxxxxxxx????xxxxxxxxxx?xxxxxxxxx?x????xxxx?xxx??xxxxxxxx????xxxx????xx"
                );

                return patternFindLoc;
            }
    );
#endif

    //We need the allocator before we run the command scanning because the logging calls need r_string allocations
    const uintptr_t allocatorVtablePtr = future_allocatorVtablePtr.get();
    const char* test = memory_utility::get_RTTI_name(allocatorVtablePtr);
    assert(strcmp(test, ".?AVMemTableFunctions@@") == 0);
    _allocator.genericAllocBase = allocatorVtablePtr;

    /*
        Unary Hashmap

        This is a hashmap using a key->bucket system, so an array of arrays keyed by a hash.
        We don't give a fuck about that though, we just want to iterate through all of the
        buckets and in turn each item in the bucket, because they are our operator entries.
        */
    for (auto& it : _game_state_ptr->_scriptFunctions)
    {
        for (auto& entry : it)
        {
            mafia::game_types::UnaryEntry new_entry {};
            new_entry.op = entry._operator;
            new_entry.procedure_ptr_addr = reinterpret_cast<uintptr_t>(&entry._operator->procedure_addr);
            new_entry.name = entry._name.data();
            /*log::info("Found unary operator: {} {} ({}) @{:x}",
                new_entry.op->return_type.type_str(), new_entry.name,
                new_entry.op->arg_type.type_str(), reinterpret_cast<uintptr_t>(new_entry.op->procedure_addr));*/
            _unary_operators[entry._name2].push_back(new_entry);
        }
    }

    /*
    Binary Hashmap
    */
    for (auto& it : _game_state_ptr->_scriptOperators)
    {
        for (auto& entry : it)
        {
            mafia::game_types::BinaryEntry new_entry {};
            new_entry.op = entry._operator;
            new_entry.procedure_ptr_addr = reinterpret_cast<uintptr_t>(&entry._operator->procedure_addr);
            new_entry.name = entry._name.data();
            /*log::debug("Found binary operator: {} ({}) {} ({}) @{:x}",
                new_entry.op->return_type.type_str(), new_entry.op->arg1_type.type_str(), new_entry.name,
                new_entry.op->arg2_type.type_str(), reinterpret_cast<uintptr_t>(new_entry.op->procedure_addr));*/
            _binary_operators[entry._name2].push_back(new_entry);
        }
    }

    /*
    Nular Hashmap
    */
    for (auto& entry : _game_state_ptr->_scriptNulars)
    {
        mafia::game_types::NularEntry new_entry {};
        new_entry.op = entry._operator;
        new_entry.procedure_ptr_addr = reinterpret_cast<uintptr_t>(&entry._operator->procedure_addr);
        new_entry.name = entry._name.data();
        //log::debug("Found nular operator: {} {} @{:x}", new_entry.op->return_type.type_str(), new_entry.name, reinterpret_cast<uintptr_t>(new_entry.op->procedure_addr));
        _nular_operators[entry._name2].push_back(new_entry);
    }

    //GameData pool allocators
    for (auto& entry : _game_state_ptr->_scriptTypes)
    {
        if (!entry->_createFunction) continue; //Some types don't have create functions. Example: VECTOR.
#if _WIN64 || __X86_64__
        auto baseOffset = 0x7;
        if (entry->_name == "CODE"sv) baseOffset += 2;

        auto instructionPointer = reinterpret_cast<uintptr_t>(entry->_createFunction) + baseOffset + 0x4;
        auto offset = *reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(entry->_createFunction) + baseOffset);
        uintptr_t poolAlloc = /*reinterpret_cast<uintptr_t>*/(instructionPointer + offset);
#else
        #ifdef __linux__
            uintptr_t poolAlloc = 1;
#else
            auto p1 = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(entry->_createFunction) + 0x3);
            uintptr_t poolAlloc = *reinterpret_cast<uintptr_t*>(p1);
#endif
#endif
        //log::debug("{} {} {}", entry->_localizedName, entry->_javaFunc, entry->_readableName);
        //log::debug("Found Type operator: {} create@{:x} pool@{:x}", entry->_name, reinterpret_cast<uintptr_t>(entry->_createFunction), poolAlloc);
        //log::flush();

        const auto type = game_types::from_string(entry->_name);
        if (poolAlloc && type != game_types::GameDataType::end)
        {
            _allocator._poolAllocs[static_cast<size_t>(type)] = reinterpret_cast<RVPoolAllocator*>(poolAlloc);
            _sqf_register_funcs._types[static_cast<size_t>(type)] = entry;
        }
    }

    //File Banks
#ifndef __linux__
    //_sqf_register_funcs._file_banks = future_fileBanks.get(); //fixed in 1.76. broken again in prof v1
#endif

    _sqf_register_funcs._type_vtable = _binary_operators["arrayintersect"sv].front().op->arg1_type.get_vtable();
    _sqf_register_funcs._compoundtype_vtable = _unary_operators["isnil"sv].front().op->arg_type.compound_type->get_vtable();
    _sqf_register_funcs._gameState = game_state_addr;

#ifndef __linux__
    _allocator.poolFuncAlloc = future_poolFuncAlloc.get();
    _allocator.poolFuncDealloc = future_poolFuncDealloc.get();
#endif
    _allocator.gameState = _game_state_ptr;
#if _WIN32

    //via profile context "scrpt"
    evaluate_script_function = future_evaluateScript.get();
    varset_function = future_varSetLocal.get();

    if (evaluate_script_function)
    {
        _allocator.evaluate_func = [](const game_data::Code& code, void* ns, const String& name) -> GameValue {
            typedef GameState* (__thiscall* evaluate_func)(GameState* gs, GameValue& ret, const String& code,
                                                            void* instruction_list, void* context, void* ns,
                                                            const String& name);
            auto func = reinterpret_cast<evaluate_func>(mafia::controller()->get_loader()->evaluate_script_function);

            struct contextType
            {
                bool _local;
                bool _nilerror;
            } c {false, true};
            GameValue ret;
            func(mafia::controller()->get_loader()->_game_state_ptr, ret, code.code_string, (void*) &code.instructions,
                 &c, ns, name);
            return ret;
        };
    }
#endif
}

uintptr_t Loader::find_game_state(uintptr_t stack_base)
{
    auto checkValid = [](uintptr_t base) -> bool {
        if (memory_utility::ptr_check(reinterpret_cast<void*>(base)))
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
        if (memory_utility::ptr_check(reinterpret_cast<void*>(size_check_type->p1)))
        { return false; }
        if (memory_utility::ptr_check(reinterpret_cast<void*>(size_check_type->p2)))
        { return false; }
        if (memory_utility::ptr_check(reinterpret_cast<void*>(size_check_type->p3)))
        { return false; }
        if (memory_utility::ptr_check(reinterpret_cast<void*>(size_check_type->p4)))
        { return false; }
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

game_types::GameState* Loader::get_game_state()
{
    return _game_state_ptr;
}
