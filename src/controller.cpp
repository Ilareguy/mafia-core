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
 * File created by Anthony Ilareguy on 23/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "controller.h"

mafia::Controller::Controller() = default;

mafia::Controller::~Controller() = default;

void mafia::Controller::_initialize()
{
    if (_initialized) return;
    do_initialize();
    _initialized = true;
}

mafia::TaskDispatcherBase::TaskDispatcherBase(mafia::Controller* const c) : _controller(c) {}

mafia::TaskDispatcherBase::~TaskDispatcherBase() = default;
