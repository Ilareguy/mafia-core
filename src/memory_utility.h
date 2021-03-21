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
 * Functions related to searching and finding addresses in process memory for values, function pointers, variables, etc.
 *
 ********************************************************
 * 
 * This file is derived from the Intercept project.
 * See https://github.com/intercept/intercept
 * See https://github.com/intercept/intercept/blob/master/src/host/loader/loader.cpp
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_MEMORY_UTILITY_H
#define DEF_MAFIA_CORE_MEMORY_UTILITY_H

#include <string_view>

// fwd
namespace mafia
{
    class SQFFunction;
    struct AllocatorInfo;
}

namespace mafia::memory_utility
{

    /**
     * Initializes the memory utility. This needs to be called once when the project is loaded into process memory by
     * Arma.
     */
    void init();

    AllocatorInfo* get_allocator();

    /**
     * Finds and returns an SQFFunction referencing the SQF function with the given function name.
     * @param function_name The function to find.
     * @param arg_signature Function signature.
     * @return A valid, callable SQFFunction object if the function was found; an invalid, un-callable SQFFunction
     * object otherwise.
     */
    SQFFunction& find(std::string_view function_name, std::string_view arg_signature);
}

#endif // DEF_MAFIA_CORE_MEMORY_UTILITY_H
