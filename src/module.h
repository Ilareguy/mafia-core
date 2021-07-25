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


#ifndef DEF_MAFIA_CORE_MODULE_H
#define DEF_MAFIA_CORE_MODULE_H

#include "runtime/mafia_module.h"
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

namespace mafia
{
    class Runtime;

    class Module: public ::mafia::runtime::Module
    {
    public:
        struct DirectoryNotFoundError: public std::exception
        {
            explicit DirectoryNotFoundError(char const* const m): std::exception(m) {}
        };

        struct MissingConfigFileError: public std::exception
        {
            explicit MissingConfigFileError(char const* const m): std::exception(m) {}
        };

        struct MissingMetaFileError: public std::exception
        {
            explicit MissingMetaFileError(char const* const m): std::exception(m) {}
        };

        struct InvalidMetaFileError: public std::exception
        {
            explicit InvalidMetaFileError(char const* const m): std::exception(m) {}
        };

        struct RuntimeModuleInitializationError: public std::exception
        {
            explicit RuntimeModuleInitializationError(char const* const m): std::exception(m) {}
        };

    public:
        /**
         * Attempts to load a module from the given directory. Throws one of the errors defined in this class if the
         * module could not be loaded.
         * @param dir Path to a directory that should contain a module.
         * @param runtime The runtime that will handle this module.
         */
        static std::shared_ptr<Module> try_load(std::string dir, Runtime& runtime);

    public:
        explicit Module(std::string dir);
        virtual ~Module();

        [[nodiscard]] const char* name() const override;
        [[nodiscard]] const char* directory_path() const override;

    public:
        nlohmann::json _config;
        std::string _name;
        std::string _tag;
        const std::string _directory_path;

    };
}

#endif // DEF_MAFIA_CORE_MODULE_H
