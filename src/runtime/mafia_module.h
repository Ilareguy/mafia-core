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
 * File created by Anthony Ilareguy on 2021-07-25.
 * [File Description]
 *
 ********************************************************
 *
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_RUNTIME_MAFIA_MODULE_H
#define DEF_MAFIA_CORE_RUNTIME_MAFIA_MODULE_H

namespace mafia::runtime
{
    class Module
    {
    public:

        [[nodiscard]] virtual const char* name() const = 0;

        /**
         * @return A path pointing to the directory containing a module's files.
         */
        [[nodiscard]] virtual const char* directory_path() const = 0;
    };
}

#endif // DEF_MAFIA_CORE_RUNTIME_MAFIA_MODULE_H