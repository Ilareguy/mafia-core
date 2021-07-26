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
 * File created by Anthony Ilareguy on 28/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_RUNTIME_ERRORS_H
#define DEF_MAFIA_CORE_RUNTIME_ERRORS_H

#include <cstring>

namespace mafia::runtime
{
    // Don't derive from std::exception because these objects will be shared across DLL boundaries.
    struct ErrorBase
    {
        explicit ErrorBase(char const* const m) noexcept
        {
            const auto s = strlen(m) + 1;
            _m = new char[s];
            strcpy_s(_m, s, m);
        }

        ~ErrorBase()
        {
            delete[] _m;
        }

        [[nodiscard]] inline const char* what() const { return _m; }

    private:
        char* _m;
    };

    struct ResultBase
    {
        explicit ResultBase(bool is_successful) noexcept: _s(is_successful) {}

        // @TODO implicit conversion to bool

        [[nodiscard]] inline bool is_success() const { return _s; }

    private:
        const bool _s;
    };

    struct Result: public ErrorBase, public ResultBase
    {
        // Indicate success
        explicit Result() noexcept: ErrorBase(""), ResultBase(true) {}

        static inline Result success() { return Result(); }

        // Indicate failure/error
        explicit Result(char const* const m) noexcept: ErrorBase(m), ResultBase(false) {}

        static inline Result error(char const* const message) { return Result(message); }
    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_ERRORS_H
