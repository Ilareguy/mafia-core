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

#include "mafia.h"
#include "mafia_private.h"
#include "rv_controller.h"

namespace mafia::_private
{
    bool exiting {false};
    std::shared_ptr<RVController> controller {nullptr};
}

bool mafia::is_exiting()
{
    return _private::exiting;
}

void mafia::exit()
{
    _private::exiting = true;
    if (_private::controller)
    {
        _private::controller->shutdown();
        _private::controller = nullptr;
    }
}

std::shared_ptr<mafia::RVController> mafia::controller()
{
    return _private::controller;
}

void mafia::_private::init_controller(uintptr_t stack_base)
{
    _private::controller = std::make_shared<mafia::RVController>();
    _private::controller->initialize(stack_base);
}
