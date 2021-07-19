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

#include "ssh_server.h"
#include "logging.h"
#include <libssh/libssh.h>

using namespace mafia;

SSHServer::SSHServer(std::string_view username, std::string_view password, unsigned int port):
        _ssh_bind {ssh_bind_new()},
        _session {ssh_new()}
{
    ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_DSAKEY, R"(@mafia/ssh_dsa)");
    ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_RSAKEY, R"(@mafia/ssh_rsa)");
    ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_BINDPORT, &port);

    if (ssh_bind_listen(_ssh_bind) < 0)
    {
        log::error("Error initializing SSH server: {}", ssh_get_error(_ssh_bind));
    }

    int r = ssh_bind_accept(_ssh_bind, _session);
    if (r == SSH_ERROR)
    {
        log::error("Error accepting SSH connection: {}", ssh_get_error(_ssh_bind));
    }

    if (ssh_handle_key_exchange(_session))
    {
        log::error("ssh_handle_key_exchange error: {}", ssh_get_error(_session));
    }
}

SSHServer::~SSHServer()
{
    ssh_disconnect(_session);
    ssh_bind_free(_ssh_bind);
    ssh_finalize();
}

bool SSHServer::auth(std::string_view username, std::string_view password)
{
    // whatever
    return ((username == _username) && (password == _password));
}
