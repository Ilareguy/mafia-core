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
 * File created by Anthony Ilareguy on 2021-07-24.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_MAFIA_RUNTIME_API_H
#define DEF_MAFIA_CORE_MAFIA_RUNTIME_API_H

namespace mafia
{
    class RuntimeAPI
    {
    public:
        RuntimeAPI() = default;
        virtual ~RuntimeAPI() = default;

        virtual void initialize() = 0;
        virtual void shutdown() = 0;
    };
}

#endif // DEF_MAFIA_CORE_MAFIA_RUNTIME_API_H
