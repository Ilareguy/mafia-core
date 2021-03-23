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

namespace mafia
{
    bool is_exiting();
    void exit();

    constexpr uint8_t version_major = 0;
    constexpr uint8_t version_minor = 0;
    constexpr uint8_t version_revision = 1;
    constexpr const char* version_suffix = "-alpha";

    /// @private
    /*extern "C" DLLEXPORT void CDECL handle_unload_internal()
    {
        exiting = true;
    }*/
}

#endif //DEF_MAFIA_CORE_MAFIA_H
