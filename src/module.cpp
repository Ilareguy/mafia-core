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
#include "runtime.h"
#include <utility>
#include <boost/filesystem.hpp>

std::shared_ptr<mafia::Module> mafia::Module::try_load(std::string dir, mafia::Runtime& runtime)
{
    namespace fs = boost::filesystem;
    constexpr auto module_config_filename = "config.json";
    constexpr auto module_meta_filename = "meta.json";
    constexpr auto modules_path = "@mafia/modules/";
    fs::path module_path {modules_path};
    module_path = module_path / dir;

    // Directory exists?
    if (!fs::is_directory(module_path))
    {
        throw Module::DirectoryNotFoundError(
                fmt::format("Could not access directory `{}`", module_path.string()).c_str());
    }

    // config.json exists
    if (!fs::is_regular_file(module_path / module_config_filename))
    {
        throw Module::MissingConfigFileError(fmt::format("{} does not exist", module_config_filename).c_str());
    }

    // meta.json exists?
    if (!fs::is_regular_file(module_path / module_meta_filename))
    {
        throw Module::MissingMetaFileError(fmt::format("{} does not exist", module_config_filename).c_str());
    }

    auto new_module = std::make_shared<Module>(module_path.string());

    // Load meta data
    try
    {
        std::ifstream meta_in {fs::path {module_path / module_meta_filename}.string()};
        nlohmann::json meta {};
        meta_in >> meta;
        new_module->_name = meta.at("name");
        new_module->_tag = meta.at("tag");
    }
    catch (const std::exception& e)
    {
        throw Module::InvalidMetaFileError(fmt::format("Error parsing {}: {}", module_meta_filename, e.what()).c_str());
    }

    // Load config data
    try
    {
        std::ifstream config_in {fs::path {module_path / module_config_filename}.string()};
        config_in >> new_module->_config;
    }
    catch (const std::exception& e)
    {
        throw Module::InvalidMetaFileError(
                fmt::format("Error parsing {}: {}", module_config_filename, e.what()).c_str());
    }

    // Initialize module with the Runtime
    auto runtime_res = runtime.api().load_module(*new_module);
    if(!runtime_res.is_success())
    {
        throw Module::RuntimeModuleInitializationError(runtime_res.what());
    }

    return new_module;
}

mafia::Module::Module(std::string dir): _directory_path(std::move(dir))
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
