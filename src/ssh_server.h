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
#include <thread>
#include <queue>
#include <mutex>
#include <cxxopts.hpp>

namespace mafia
{
    class SSHServer
    {
    private:
        /**
         * Parsed command-line arguments.
         */
        struct Arguments
        {
            explicit Arguments(std::vector<std::string>&& argv_vec);
            ~Arguments();
            char** argv;
            int argc;
            std::vector<std::string> argv_vec;
        };
        static std::unique_ptr<SSHServer::Arguments> _parse_arguments(std::string_view);

    public:
        SSHServer(std::string_view username, std::string_view password, unsigned int port);
        ~SSHServer();

        /**
         * Sends the given message to the connected remote SSH client. Does nothing if no client is currently connected.
         * @param message Message to send.
         */
        void send(std::string_view message);

    private:
        bool _auth(std::string_view username, std::string_view password);
        void _ssh_thread(unsigned int port);
        bool _ssh_accept_connection();
        bool _ssh_open_channel();
        bool _ssh_open_shell_channel();
        std::string _process_message(std::string_view);
        void _do_send(const std::string&); // Only call from SSH thread. Use ``send()`` otherwise, which is thread-safe
        void _init_interface();

    private:
        std::string _username, _password;
        ssh_session _session {nullptr};
        ssh_bind _ssh_bind {nullptr};
        ssh_message _message {nullptr};
        ssh_channel _channel {nullptr};
        std::thread _ssh_worker;
        bool _stop_ssh {false};
        std::queue<std::string> _messages_to_send;
        std::recursive_mutex _m;
        cxxopts::Options _ssh_interface;
    };
}

#endif // DEF_MAFIA_CORE_SSH_SERVER_H
