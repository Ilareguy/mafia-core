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
 * File created by Anthony Ilareguy on 28/02/2021.
 * [File Description]
 *
 ********************************************************
 *
 * [Credits]
 *
 ********************************************************/

#include "dll_handle.h"
#include <dlfcn.h>
#include <fmt/format.h>

mafia::DLLHandle::DLLHandle(std::string_view sv)
{
    // Open dll_handle
    _raw_handle = dlopen(sv.data(), RTLD_NOW | RTLD_GLOBAL);

    // Fail if a dll_handle could not be opened properly
    if (nullptr == _raw_handle)
    {
        throw DLLOpenError {fmt::format("Could not load dynamic library file \"{}\".", sv).c_str()};
    }
}

mafia::DLLHandle::~DLLHandle()
{
    dlclose(_raw_handle);
}
