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
 * File created by Anthony Ilareguy on 20/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_SQF_FUNCTION_H
#define DEF_MAFIA_CORE_SQF_FUNCTION_H

namespace mafia
{
    /**
     * Defines a single SQF function object, which can be invoked.
     */
    class SQFFunction
    {
        /**
         * @return true if the object is a valid & callable function; false otherwise.
         */
        bool is_valid() const;
    };
}

#endif //DEF_MAFIA_CORE_SQF_FUNCTION_H
