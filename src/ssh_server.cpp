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
#include "ssh/module.h"
#include "ssh/javascript.h"

using namespace mafia;

SSHServer::Arguments::Arguments(std::vector<std::string>&& argv_vec_): argv_vec(std::move(argv_vec_))
{
    argc = argv_vec.size();

    argv = new char* [argv_vec.size()];
    for (int i = 0; i < argv_vec.size(); i++)
    {
        const auto size = argv_vec[i].size();
        argv[i] = new char[size + 1];
        strcpy_s(argv[i], size + 1, argv_vec[i].c_str());
    }
}

SSHServer::Arguments::~Arguments()
{
    for (int i = 0; i < argc; i++)
    {
        delete[] argv[i];
    }
    delete[] argv;
}

std::unique_ptr<SSHServer::Arguments> SSHServer::_parse_arguments(std::string_view cmd)
{
    std::vector<std::string> parsed_arguments;
    std::ostringstream current_argument;
    bool in_quotes = false;

    auto complete_argument = [&parsed_arguments, &current_argument]() {
        auto current_argument_str = current_argument.str();

        if (!current_argument_str.empty())
        {
            parsed_arguments.push_back(current_argument_str);
            current_argument = std::ostringstream();
        }
    };

    for (const char& c: cmd)
    {
        if (c == '"')
        {
            in_quotes = !in_quotes;
            if (!in_quotes)
            {
                complete_argument();
            }
        }
        else if (!in_quotes && c == ' ')
        {
            complete_argument();
        }
        else
        {
            current_argument << c;
        }
    }
    complete_argument();
    return std::make_unique<SSHServer::Arguments>(std::move(parsed_arguments));
}

SSHServer::SSHServer(std::string_view username, std::string_view password, unsigned int port):
        AsynchronousTaskExecutor(1),
        _username(username),
        _password(password)
{
    _init_interfaces();
    log::info("Starting SSH worker thread");
    _ssh_worker = std::thread(&SSHServer::_ssh_thread, this, port);
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

void SSHServer::_init_interfaces()
{
    _ssh_command_interfaces[SSH_INTERFACE_MODULE] = std::make_unique<ssh::ModuleInterface>();
    _ssh_command_interfaces[SSH_INTERFACE_JAVASCRIPT] = std::make_unique<ssh::JavascriptInterface>();

    for (auto& s : _ssh_command_interfaces)
    {
        s->init();
    }
}

void SSHServer::send(const std::string_view message)
{
    if (!ssh_is_connected(_session))
    {
        return;
    }

    std::lock_guard l {_m};
    _messages_to_send.push(std::string {message});
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
            ssh_message_subtype(_message) == /*SSH_CHANNEL_REQUEST_PTY*/ SSH_CHANNEL_REQUEST_SHELL)
        {
            ssh_message_channel_request_reply_success(_message);
            return true;
        }
        ssh_message_reply_default(_message);
        ssh_message_free(_message);
    } while (_message);

    return false;
}

void SSHServer::_ssh_thread(unsigned int port)
{
    do
    {
        char buf[2048];
        int i = 0;
        int r = 0;

        _ssh_bind = ssh_bind_new();
        _session = ssh_new();

        ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_DSAKEY, R"(@mafia/ssh_dsa)");
        ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_RSAKEY, R"(@mafia/ssh_rsa)");
        ssh_bind_options_set(_ssh_bind, SSH_BIND_OPTIONS_BINDPORT, &port);

        if (ssh_bind_listen(_ssh_bind) < 0)
        {
            log::error("SSH Thread: Error initializing SSH server: {}", ssh_get_error(_ssh_bind));
        }

        /*log::info("SSH Thread: Waiting for user authentication...");
        log::flush();*/

        if (!_ssh_accept_connection())
        {
            log::error("SSH Thread: Authentication error: {}", ssh_get_error(_session));
        }

        /*log::info("SSH Thread: Authentication successful; opening channel...");
        log::flush();*/

        if (!_ssh_open_channel() || !_ssh_open_shell_channel())
        {
            log::error("SSH Thread error: {}", ssh_get_error(_session));
        }

        /*log::info("SSH Thread: Connection established!");
        log::flush();*/

        send("Welcome!");
        bool send_receive_loop {true};

        do
        {
            // Message to send?
            {
                std::lock_guard l {_m};
                if (!_messages_to_send.empty())
                {
                    const auto message = _messages_to_send.front();
                    _messages_to_send.pop();
                    _do_send(message);
                }
            }

            i = ssh_channel_read_timeout(_channel, buf, 2048, 0, 10);
            if (i > 0)
            {
                buf[i - 1] = '\0';
                std::string received_message {buf};
                const auto response = _process_message(std::string {buf});
                _do_send(response);
            }
            else if (i == SSH_ERROR)
            {
                log::error("SSH Thread: {}", ssh_get_error(_session));
                send_receive_loop = false;
            }
        } while (send_receive_loop && !_stop_ssh);

        ssh_disconnect(_session);
        ssh_bind_free(_ssh_bind);
        ssh_free(_session);
    } while (!_stop_ssh);

    ssh_finalize();
    // log::info("Stopping SSH worker thread");
}

std::string SSHServer::_process_message(std::string_view raw_message)
{
    auto args = _parse_arguments(raw_message);
    if (args->argc == 0)
    {
        return "";
    }

    const auto command_name = args->argv_vec.front();

    try
    {
        if (command_name == "help")
        {
            return "(Not yet implemented)";
        }

        for (auto& inter : _ssh_command_interfaces)
        {
            if (command_name == inter->getCommandName())
            {
                return inter->execute(command_name, args->argc, args->argv, *this);
            }
        }

        return fmt::format("Unrecognized command \"{}\".", command_name);
    }
    catch (const cxxopts::OptionParseException& e)
    {
        return fmt::format("Error while parsing options: {}", e.what());
    }
}

void SSHServer::_do_send(const std::string& m)
{
    ssh_channel_write(_channel, m.c_str(), m.length());
    ssh_channel_write(_channel, "\n\r", 2);
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
