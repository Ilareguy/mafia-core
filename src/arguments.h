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

#ifndef DEF_MAFIA_CORE_ARGUMENTS_H
#define DEF_MAFIA_CORE_ARGUMENTS_H

#include <cstdint>
#include <vector>
#include <string>

namespace mafia
{
    class ArgumentsAccessor
    {
    public:
        ArgumentsAccessor(const uint32_t index, const std::vector<std::string>& ar) noexcept;
        const std::string& as_string() const;
        operator const std::string&() const;
        static float to_float(const std::string& val);
        float as_float() const;
        operator float() const;
        int as_int() const;
        operator int() const;
        int as_uint32() const;
        operator uint32_t() const;

    protected:
        const uint32_t _index;
        const std::vector<std::string>& _args;
    };

    class Arguments
    {
    public:
        Arguments(const std::string& str);
        size_t size() const noexcept;
        const ArgumentsAccessor operator[](int index) const noexcept;
        static float to_float(const std::string& val);
        const std::string& as_string();
        float as_float();
        int as_int();
        int as_uint32();
        const std::string& as_string(uint32_t _index) const;
        float as_float(uint32_t _index) const;
        int as_int(uint32_t _index) const;
        int as_uint32(uint32_t _index) const;
        const std::string& get() const noexcept;
        std::string create(const std::string& command) const;

    public:
        static std::string create(const std::string& command, const Arguments& args);


    protected:
        std::vector<std::string> _args;
        const std::string _original;
        uint32_t _internal_index;
    };
}


#endif //DEF_MAFIA_CORE_ARGUMENTS_H
