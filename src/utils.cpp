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
 * File created by Anthony Ilareguy on 22/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "utils.h"
#include <sstream>
#include <cassert>

std::vector<std::string>& mafia::split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string_view>& mafia::split(std::string_view s, char delim, std::vector<std::string_view>& elems)
{
    std::string::size_type lastPos = 0;
    const auto length = s.length();

    while (lastPos < length + 1)
    {
        std::string::size_type pos = s.find_first_of(delim, lastPos);
        if (pos == std::string::npos)
        {
            pos = length;
        }

        //if (pos != lastPos || !trimEmpty)
        elems.emplace_back(s.data() + lastPos, pos - lastPos);

        lastPos = pos + 1;
    }

    return elems;
}

std::vector<std::string> mafia::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string_view> mafia::split(std::string_view s, char delim)
{
    std::vector<std::string_view> elems;
    split(s, delim, elems);
    return elems;
}

std::string& mafia::ltrim(std::string& s)
{
    s.erase(
            s.begin(),
            std::find_if(s.begin(), s.end(), [](const char _char) noexcept { return !std::isspace(_char); }));
    return s;
}

std::string& mafia::rtrim(std::string& s)
{
    s.erase(
            std::find_if(s.rbegin(), s.rend(), [](const char _char) noexcept { return !std::isspace(_char); }).base(),
            s.end());
    return s;
}

std::string& mafia::trim(std::string& s)
{
    return ltrim(rtrim(s));
}

void mafia::runtime_assert(bool result)
{
    assert(result);
    if (!result) {
        throw std::exception("Mafia Assertion failed", -1);
    }
}

void mafia::runtime_assert(bool result, const uint32_t code, const std::string& text)
{
    assert(result);
    if (!result) {
        throw std::exception(text.c_str(), code);
    }
}
