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
 * File created by Anthony Ilareguy on 22/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "logging.h"
#include "logging_private.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/logger.h>
#include <memory>

namespace mafia::log::_private
{
    static std::shared_ptr<spdlog::logger> logger {nullptr};
    static bool initialized {false};
}

void mafia::log::_private::init()
{
    // Create a file rotating logger with 2mb size max and 3 rotated files
    auto max_size = 1048576 * 10;
    auto max_files = 3;
    logger = spdlog::rotating_logger_mt("mafia-core", "@mafia/logs/mafia.log", max_size, max_files);

#ifdef MAFIA_DEBUG
    logger->set_level(spdlog::level::debug);
#endif

    initialized = true;
}

void mafia::log::_private::shutdown()
{
    logger->flush();
    logger = nullptr;
    initialized = false;
}

void mafia::log::_info(std::string_view s)
{
    _private::logger->info(s);
}

void mafia::log::_debug(std::string_view s)
{
    _private::logger->debug(s);
}

void mafia::log::_warn(std::string_view s)
{
    _private::logger->warn(s);
}

void mafia::log::_error(std::string_view s)
{
    _private::logger->error(s);
}

void mafia::log::_critical(std::string_view s)
{
    _private::logger->critical(s);
}

void mafia::log::_trace(std::string_view s)
{
    _private::logger->trace(s);
}

void mafia::log::flush()
{
    _private::logger->flush();
}
