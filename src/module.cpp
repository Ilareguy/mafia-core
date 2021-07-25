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
 * File created by Anthony Ilareguy on 2021-07-25.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "module.h"
#include <utility>

std::shared_ptr<mafia::Module> mafia::Module::try_load(std::string dir, mafia::Runtime& runtime)
{
    throw Module::DirectoryNotFoundError("yolo");
}

mafia::Module::Module(std::string dir) : _directory_path(std::move(dir))
{
    //
}

mafia::Module::~Module() = default;

const char* mafia::Module::name() const
{
    return _name.c_str();
}

const char* mafia::Module::directory_path() const
{
    return _directory_path.c_str();
}
