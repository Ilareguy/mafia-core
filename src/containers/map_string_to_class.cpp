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

#include "map_string_to_class.h"

using namespace mafia::containers;

unsigned int rv_map_hash_string_case_sensitive(std::string_view key, int hashValue) noexcept
{
    for (auto& it : key)
    {
        hashValue = hashValue * 33 + static_cast<unsigned char>(it);
    }
    return hashValue;
}

unsigned int mafia::containers::rv_map_hash_string_case_sensitive(std::string_view key, int hashValue) noexcept
{
    for (auto& it : key)
    {
        hashValue = hashValue * 33 + static_cast<unsigned char>(tolower(it));
    }
    return hashValue;
}

unsigned int mafia::containers::rv_map_hash_string_case_insensitive(std::string_view key, int hashValue) noexcept
{
    for (auto& it : key)
    {
        hashValue = hashValue * 33 + static_cast<unsigned char>(tolower(it));
    }
    return hashValue;
}

unsigned int map_string_to_class_trait::hash_key(std::string_view key) noexcept
{
    return rv_map_hash_string_case_sensitive(key);
}

bool map_string_to_class_trait::compare_keys(std::string_view k1, std::string_view k2) noexcept
{
    if (k1.length() != k2.length())
    { return false; }
    return k1.compare(k2) == 0;
}

unsigned int map_string_to_class_trait_case_insensitive::hash_key(std::string_view key) noexcept
{
    return rv_map_hash_string_case_insensitive(key);
}

bool map_string_to_class_trait_case_insensitive::compare_keys(std::string_view k1, std::string_view k2) noexcept
{
    if (k1.length() != k2.length())
    { return false; }

    return std::equal(
            k1.begin(), k1.end(),
            k2.begin(), [](unsigned char l, unsigned char r) { return l == r || tolower(l) == tolower(r); }
    );
}
