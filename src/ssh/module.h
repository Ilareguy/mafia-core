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


#ifndef DEF_MAFIA_CORE_SSH_COMMAND_MODULE_H
#define DEF_MAFIA_CORE_SSH_COMMAND_MODULE_H

#include "command.h"

namespace mafia::ssh
{
    class ModuleInterface : public Command
    {
    public:
        ModuleInterface();
        virtual ~ModuleInterface();

    protected:
        virtual std::string execute_command(const std::string& command_name, const cxxopts::ParseResult& args) override;
        virtual void init_ssh_interface(cxxopts::OptionAdder&&) override;
    };
}

#endif // DEF_MAFIA_CORE_SSH_COMMAND_MODULE_H
