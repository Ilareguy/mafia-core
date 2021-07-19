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
 * File created by Anthony Ilareguy on 19/07/2021.
 * [File Description]
 *
 ********************************************************
 *
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_SSH_SERVER_H
#define DEF_MAFIA_CORE_SSH_SERVER_H

#include <string_view>
#include <libssh/server.h>

namespace mafia
{
    class SSHServer
    {
    public:
        SSHServer(std::string_view username, std::string_view password, unsigned int port);
        ~SSHServer();

        bool auth(std::string_view username, std::string_view password);

    private:
        std::string_view _username, _password;
        ssh_session _session;
        ssh_bind _ssh_bind;
        ssh_message _message;
        ssh_channel _channel {nullptr};
    };
}

#endif //DEF_MAFIA_CORE_SSH_SERVER_H
