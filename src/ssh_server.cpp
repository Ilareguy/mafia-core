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
        _session {ssh_new()},
        _message {},
        _username(username),
        _password(password)
{
    ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_DSAKEY, R"(@mafia/ssh_dsa)");
    ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_RSAKEY, R"(@mafia/ssh_rsa)");
    ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_BINDPORT, &port);
    log::info("Starting SSH worker thread");
    _ssh_worker = std::thread(&SSHServer::_ssh_thread, this);
}

SSHServer::~SSHServer()
{
    if (nullptr != _session)
    {
        ssh_disconnect(_session);
        _session = nullptr;
    }
    _stop_ssh = true;

    if (_ssh_worker.joinable())
    {
        _ssh_worker.join();
    }
}

bool SSHServer::_ssh_accept_connection()
{
    int auth = 0;

    const int bind_result = ssh_bind_accept(_ssh_bind, _session);
    if (bind_result == SSH_ERROR)
    {
        log::error("SSH Thread: Error accepting SSH connection: {}", ssh_get_error(_ssh_bind));
    }

    if (ssh_handle_key_exchange(_session))
    {
        log::error("SSH Thread: ssh_handle_key_exchange error: {}", ssh_get_error(_session));
    }

    do
    {
        _message = ssh_message_get(_session);
        if (!_message)
        {
            break;
        }

        switch (ssh_message_type(_message))
        {
            case SSH_REQUEST_AUTH:
                switch (ssh_message_subtype(_message))
                {
                    case SSH_AUTH_METHOD_PASSWORD:
                        if (_auth(ssh_message_auth_user(_message), ssh_message_auth_password(_message)))
                        {
                            auth = 1;
                            ssh_message_auth_reply_success(_message, 0);
                            break;
                        }
                        // not authenticated, send default message
                    case SSH_AUTH_METHOD_NONE:
                    default:ssh_message_auth_set_methods(_message, SSH_AUTH_METHOD_PASSWORD);
                        ssh_message_reply_default(_message);
                        break;
                }
                break;
            default:ssh_message_reply_default(_message);
        }
        ssh_message_free(_message);
    } while (!auth);

    if (!auth)
    {
        return false;
    }

    return true;
}

bool SSHServer::_ssh_open_channel()
{
    do
    {
        _message = ssh_message_get(_session);
        if (_message)
        {
            switch (ssh_message_type(_message))
            {
                case SSH_REQUEST_CHANNEL_OPEN:
                    if (ssh_message_subtype(_message) == SSH_CHANNEL_SESSION)
                    {
                        _channel = ssh_message_channel_request_open_reply_accept(_message);
                        break;
                    }
                default:ssh_message_reply_default(_message);
            }
            ssh_message_free(_message);
        }
    } while (_message && !_channel);

    if (!_channel)
    {
        return false;
    }

    return true;
}

bool SSHServer::_ssh_open_shell_channel()
{
    do
    {
        _message = ssh_message_get(_session);
        if (_message && ssh_message_type(_message) == SSH_REQUEST_CHANNEL &&
            ssh_message_subtype(_message) == SSH_CHANNEL_REQUEST_SHELL)
        {
            ssh_message_channel_request_reply_success(_message);
            return true;
        }
        ssh_message_reply_default(_message);
        ssh_message_free(_message);
    } while (_message);

    return false;
}

void SSHServer::_ssh_thread()
{
    if (ssh_bind_listen(_ssh_bind) < 0)
    {
        log::error("SSH Thread: Error initializing SSH server: {}", ssh_get_error(_ssh_bind));
    }

    do
    {
        char buf[2048];
        int i = 0;
        int r = 0;

        // @FIXME When a user closes connection, it's currently impossible to re-establish a new connection

        log::info("SSH Thread: Waiting for user authentication...");
        log::flush();
        if (!_ssh_accept_connection())
        {
            log::error("SSH Thread: Authentication error: {}", ssh_get_error(_session));
        }

        log::info("SSH Thread: Authentication successful; opening channel...");
        log::flush();
        if (!_ssh_open_channel() || !_ssh_open_shell_channel())
        {
            log::error("SSH Thread error: {}", ssh_get_error(_session));
        }

        log::info("SSH Thread: Connection established!");
        log::flush();

        do
        {
            i = ssh_channel_read(_channel, buf, 2048, 0);
            if (i > 0)
            {
                ssh_channel_write(_channel, buf, i);
                /*if (write(1, buf, i) < 0)
                {
                    log::error("SSH Thread: Error writing to buffer!");
                }*/
            }
        } while (i > 0);

    } while (!_stop_ssh);

    ssh_disconnect(_session);
    ssh_bind_free(_ssh_bind);
    ssh_finalize();

    log::info("Stopping SSH worker thread");
    log::flush();
}

bool SSHServer::_auth(std::string_view username, std::string_view password)
{
    log::info(
            "SSH Thread: User {} wants to authenticate with password {}",
            username, password
    );
    log::flush();

    // whatever
    return ((username == _username) && (password == _password));
}
