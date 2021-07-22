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

#ifndef DEF_MAFIA_CORE_NON_COPYABLE_H
#define DEF_MAFIA_CORE_NON_COPYABLE_H

namespace mafia
{
    // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin
    class NonCopyable
    {
    public:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable& operator=(NonCopyable&&) = delete;

    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
    };
}

#endif // DEF_MAFIA_CORE_NON_COPYABLE_H
