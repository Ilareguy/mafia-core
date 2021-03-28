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
 * File created by Anthony Ilareguy on 24/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#ifndef DEF_MAFIA_CORE_RV_CONTROLLER_H
#define DEF_MAFIA_CORE_RV_CONTROLLER_H

#include "controller.h"
#include "arguments.h"
#include "game_types/game_data.h"
#include <functional>
#include <unordered_map>

namespace mafia
{
    class Invoker;
    class Loader;
    class SQFFunctions;
    class MissionEvents;

    /**
     * Base class for a Controller with features specific to Arma's RV engine. This includes a Loader, an Invoker, and
     * an SQFFunctions object.
     */
    class RVController
    {
    private:
        typedef std::function<std::string(mafia::Arguments& args)> RVCommandHandler_t;
        typedef std::unordered_map<std::string, RVCommandHandler_t> RVCommandHandlers_t;

    public:
        RVController();
        virtual ~RVController();
        void initialize(uintptr_t stack_base);
        void shutdown();

        [[nodiscard]] std::shared_ptr<Loader> get_loader();
        [[nodiscard]] std::shared_ptr<SQFFunctions> get_sqf_functions();
        [[nodiscard]] std::shared_ptr<Invoker> get_invoker();
        [[nodiscard]] std::shared_ptr<MissionEvents> get_mission_events();

        std::string rv_call(std::string_view command, mafia::Arguments& args);
        bool rv_signal(
                const std::string& extension_name,
                const std::string& signal_name,
                const game_types::GameValue& args
        );
        bool add_rv_command_handler(std::string_view command, RVCommandHandler_t handler);

    protected:
        //

    private:
        std::shared_ptr<Loader> _loader;
        std::shared_ptr<SQFFunctions> _sqf_functions;
        std::shared_ptr<Invoker> _invoker;
        std::shared_ptr<MissionEvents> _mission_events;
        RVCommandHandlers_t _command_handlers;
    };
}

#endif //DEF_MAFIA_CORE_RV_CONTROLLER_H
