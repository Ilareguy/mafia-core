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

#include "arguments.h"
#include "utils.h"
#include <sstream>

mafia::ArgumentsAccessor::ArgumentsAccessor(const uint32_t index, const std::vector<std::string>& ar) noexcept
        : _index(index), _args(ar) {}

const std::string& mafia::ArgumentsAccessor::as_string() const { return _args[_index]; }

mafia::ArgumentsAccessor::operator const std::string&() const { return as_string(); }

float mafia::ArgumentsAccessor::to_float(const std::string& val)
{
    float res = 0.0f;
    std::istringstream iss(val);
    iss >> res;
    return res;
}

float mafia::ArgumentsAccessor::as_float() const { return to_float(_args[_index]); }

mafia::ArgumentsAccessor::operator float() const { return as_float(); }

int mafia::ArgumentsAccessor::as_int() const { return atoi(_args[_index].c_str()); }

mafia::ArgumentsAccessor::operator int() const { return as_int(); }

int mafia::ArgumentsAccessor::as_uint32() const { return static_cast<uint32_t>(atoi(_args[_index].c_str())); }

mafia::ArgumentsAccessor::operator uint32_t() const { return as_uint32(); }

mafia::Arguments::Arguments(const std::string& str): _original(str), _internal_index(0)
{
    _args = mafia::split(str, ',');
    for (uint32_t i = 0; i < _args.size(); i++)
    {
        _args[i] = trim(_args[i]);
    }
}

size_t mafia::Arguments::size() const noexcept { return _args.size(); }

const mafia::ArgumentsAccessor mafia::Arguments::operator[](int index) const noexcept
{
    return ArgumentsAccessor(
            index, _args
    );
}

float mafia::Arguments::to_float(const std::string& val)
{
    float res = 0.0f;
    std::istringstream iss(val);
    iss >> res;
    return res;
}

const std::string& mafia::Arguments::as_string() { return _args[_internal_index++]; }

float mafia::Arguments::as_float() { return to_float(_args[_internal_index++]); }

int mafia::Arguments::as_int() { return atoi(_args[_internal_index++].c_str()); }

int mafia::Arguments::as_uint32() { return static_cast<uint32_t>(atoi(_args[_internal_index++].c_str())); }

const std::string& mafia::Arguments::as_string(uint32_t _index) const { return _args[_index]; }

float mafia::Arguments::as_float(uint32_t _index) const { return to_float(_args[_index]); }

int mafia::Arguments::as_int(uint32_t _index) const { return atoi(_args[_index].c_str()); }

int mafia::Arguments::as_uint32(uint32_t _index) const { return static_cast<uint32_t>(atoi(_args[_index].c_str())); }

const std::string& mafia::Arguments::get() const noexcept
{
    return _original;
}

std::string mafia::Arguments::create(const std::string& command) const
{
    std::stringstream ss;
    ss << command << ":";

    for (auto& v : _args)
    {
        ss << v << ",";
    }

    // Remove the trailing ,
    std::string result = ss.str();
    result.erase(result.length());

    return result;
}

std::string mafia::Arguments::create(const std::string& command, const mafia::Arguments& args)
{
    return args.create(command);
}
