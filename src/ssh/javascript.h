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
 * File created by Anthony Ilareguy on 2021-07-26.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/


#ifndef DEF_MAFIA_CORE_SSH_COMMAND_JAVASCRIPT_H
#define DEF_MAFIA_CORE_SSH_COMMAND_JAVASCRIPT_H

#include "command.h"

namespace mafia::ssh
{
    class JavascriptInterface: public Command
    {
    public:
        JavascriptInterface();
        virtual ~JavascriptInterface();

    protected:
        std::string execute_command(const std::string&, const cxxopts::ParseResult&, ::mafia::SSHServer&) override;
        void init_ssh_interface(cxxopts::OptionAdder&&, cxxopts::Options& options) override;
    };
}

#endif // DEF_MAFIA_CORE_SSH_COMMAND_JAVASCRIPT_H
