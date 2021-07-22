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
 * File created by Anthony Ilareguy on 2021-07-22.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "scheduled_command.h"

using namespace mafia;

ssh::ScheduledCommand::ScheduledCommand(QueuedFunction_t&& f, QueuedFunctionThread t):
        _function(std::move(f)), _thread(t) {}

ssh::ScheduledCommand::~ScheduledCommand() = default;

void ssh::ScheduledCommand::then(QueuedFunctionThen_t&& f)
{
    _then = std::move(f);
}
