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

#ifndef DEF_MAFIA_CORE_V_TABLE_H
#define DEF_MAFIA_CORE_V_TABLE_H

namespace mafia
{
    /*
     * This is a placeholder so i can use refcount but still have an accessible vtable pointer
     */
    class __vtable
    {
    public:
        uintptr_t _vtable {0};
    };

    class DummyVtable
    {
    public:
        virtual void dummy() {};
    };

    class RefCountDummyVtable: public __vtable, public mafia::game_types::RefCountBase
    {
    };
}

#endif //DEF_MAFIA_CORE_V_TABLE_H
