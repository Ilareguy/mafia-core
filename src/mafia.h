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

#ifndef DEF_MAFIA_CORE_MAFIA_H
#define DEF_MAFIA_CORE_MAFIA_H

#include <cstdint>
#include <memory>

namespace mafia
{
    class Controller;

    bool is_exiting();
    void exit();
    std::shared_ptr<Controller> controller();

    namespace version
    {
        constexpr uint8_t major = 0;
        constexpr uint8_t minor = 0;
        constexpr uint8_t revision = 1;
        constexpr const char* suffix = "-alpha";
    }
}

#endif //DEF_MAFIA_CORE_MAFIA_H
