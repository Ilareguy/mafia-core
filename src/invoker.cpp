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

#include "invoker.h"
#include "logging.h"
#include "rv_controller.h"
#include "mafia.h"
#include "loader.h"
#include "mission_events.h"
#include "game_types/game_data/all.h"
#include <utility>

using namespace mafia;
using namespace mafia::game_types;
using namespace std::literals::string_view_literals;

GameState* Invoker::sqf_game_state {nullptr};
bool Invoker::invoker_accessible {false};
bool Invoker::invoker_accessible_all {false};

Invoker::Invoker(RVController& c): _rv_controller(c)
{
    _rv_controller.add_rv_command_handler(
            "init_invoker"sv,
            [this](const auto& args_) { return _init_invoker(args_); }
    );
    _rv_controller.add_rv_command_handler(
            "test_invoker"sv,
            [this](const auto args_) { return _test_invoker(args_); }
    );
    _rv_controller.add_rv_command_handler(
            "invoker_begin_register"sv,
            [this](const auto args_) { return _invoker_begin_register(args_); }
    );
    _rv_controller.add_rv_command_handler(
            "invoker_register"sv,
            [this](const auto args_) { return _invoker_register(args_); }
    );
    _rv_controller.add_rv_command_handler(
            "invoker_end_register"sv,
            [this](const auto args_) { return _invoker_end_register(args_); }
    );
}

Invoker::~Invoker() = default;

std::string Invoker::_init_invoker(const mafia::Arguments& args_)
{
    _invoker_unlock init_lock(*this);
    return "OK";
}

std::string Invoker::_test_invoker(const mafia::Arguments& args_)
{
    _invoker_unlock test_lock(*this);
    mafia::game_types::GameValue res = invoke_raw("profilenamesteam"sv);
    return static_cast<std::string>(res);
}

std::string Invoker::_invoker_begin_register(const mafia::Arguments& args_)
{
    auto sqf_functions = _rv_controller.get_sqf_functions();
    sqf_functions->initialize();

    _registerTypes_function = sqf_functions->register_sqf_function(
            "mafiaRegisterTypes"sv, "",
            userFunctionWrapper<_mafia_registerTypes>,
            game_types::GameDataType::BOOL,
            game_types::GameDataType::NAMESPACE
    );

    _event_function = sqf_functions->register_sqf_function(
            "mafiaEvent"sv, "",
            userFunctionWrapper<_mafia_event>,
            game_types::GameDataType::BOOL,
            game_types::GameDataType::STRING,
            game_types::GameDataType::ARRAY
    );

    _signal_function = sqf_functions->register_sqf_function(
            "mafiaSignal"sv, "",
            userFunctionWrapper<_mafia_signal>,
            game_types::GameDataType::BOOL,
            game_types::GameDataType::ARRAY,
            game_types::GameDataType::ARRAY
    );

    _do_invoke_period_function = sqf_functions->register_sqf_function(
            "mafiaOnFrame"sv, "",
            userFunctionWrapper<_mafia_do_invoke_period>,
            game_types::GameDataType::BOOL
    );

    return "";
}

std::string Invoker::_invoker_register(const mafia::Arguments& args_)
{
    _registration_type = args_.as_string(0);
    return "true";
}

std::string Invoker::_invoker_end_register(const mafia::Arguments& args_)
{
    controller()->get_sqf_functions()->setDisabled();
    //init_file_bank_list();

    return "true";
}

Invoker::_invoker_unlock::_invoker_unlock(Invoker& instance_, bool all_threads_, bool delayed_): _unlocked(false),
                                                                                                 _instance(instance_),
                                                                                                 _all(all_threads_)
{
    if (!delayed_)
    {
        unlock();
    }
}

GameValue Invoker::invoke_raw_nolock(NularFunction function_) noexcept
{
    volatile uintptr_t arr[64];//artifical stackpushing
    arr[13] = 5;
    return function_(*Invoker::sqf_game_state);
}

GameValue Invoker::invoke_raw(std::string_view function_name_) const
{
    NularFunction function;
    if (_rv_controller.get_loader()->get_function(function_name_, function))
    {
        return invoke_raw_nolock(function);
    }
    return GameValue();
}

GameValue Invoker::invoke_raw_nolock(UnaryFunction function_, const GameValue& right_arg_) noexcept
{
    volatile uintptr_t arr[64];//artifical stackpushing
    arr[13] = 5;
    return function_(*Invoker::sqf_game_state, right_arg_);
}

GameValue
Invoker::invoke_raw(std::string_view function_name_, const GameValue& right_, const std::string& right_type_) const
{
    UnaryFunction function;
    if (_rv_controller.get_loader()->get_function(function_name_, function, right_type_))
    {
        return invoke_raw_nolock(function, right_);
    }
    return GameValue();
}

GameValue Invoker::invoke_raw(std::string_view function_name_, const GameValue& right_) const
{
    UnaryFunction function;
    if (_rv_controller.get_loader()->get_function(function_name_, function))
    {
        return invoke_raw_nolock(function, right_);
    }
    return GameValue();
}

GameValue Invoker::invoke_raw_nolock(
        BinaryFunction function_, const GameValue& left_arg_,
        const GameValue& right_arg_
) noexcept
{
    volatile uintptr_t arr[64];//artifical stackpushing
    arr[13] = 5;
    return function_(*Invoker::sqf_game_state, left_arg_, right_arg_);
}

GameValue Invoker::invoke_raw(std::string_view function_name_, const GameValue& left_, const GameValue& right_) const
{
    BinaryFunction function;
    if (_rv_controller.get_loader()->get_function(function_name_, function))
    {
        return invoke_raw_nolock(function, left_, right_);
    }
    return GameValue();
}

GameValue Invoker::invoke_raw(
        std::string_view function_name_, const GameValue& left_, const std::string& left_type_,
        const GameValue& right_, const std::string& right_type_
) const
{
    BinaryFunction function;
    if (_rv_controller.get_loader()->get_function(function_name_, function, left_type_, right_type_))
    {
        return invoke_raw_nolock(function, left_, right_);
    }
    return GameValue();
}

mafia::Invoker::_invoker_unlock::~_invoker_unlock()
{
    if (_unlocked)
    {
        std::lock_guard<std::mutex> lock(_instance._state_mutex);
        if (_all)
        {
            std::lock_guard<std::recursive_mutex> invoke_lock(_instance._invoke_mutex);
            Invoker::invoker_accessible = false;
            Invoker::invoker_accessible_all = false;
        }
        else
        {
            Invoker::invoker_accessible = false;
        }

    }
}

void mafia::Invoker::_invoker_unlock::unlock()
{
    if (!_unlocked)
    {
        if (_all)
        {
            std::unique_lock<std::recursive_mutex> invoke_lock(_instance._invoke_mutex, std::defer_lock);
            {
                std::lock_guard<std::mutex> lock(_instance._state_mutex);
                invoke_lock.lock();
                Invoker::invoker_accessible = true;
                Invoker::invoker_accessible_all = true;
            }
            _instance._invoke_condition.notify_all();
        }
        else
        {
            std::lock_guard<std::mutex> lock(_instance._state_mutex);
            Invoker::invoker_accessible = true;
        }
        _unlocked = true;
    }
}

mafia::game_types::GameValue Invoker::_mafia_do_invoke_period()
{
    return mafia::controller()->get_invoker()->do_invoke_period();
}

bool Invoker::do_invoke_period()
{
    //controller()->get_logger()->info(__FUNCTION__);
    {
        _invoker_unlock period_lock(*this, true);
        if (_thread_waiting_for_lock_count)
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        } //Sleep some, to allow other threads to catch the lock
        const long timeout = clock() + 3;
        while (_thread_count > 0 && clock() < timeout)
        { std::this_thread::sleep_for(std::chrono::microseconds(20)); }
    }
    return true;
}

mafia::game_types::GameValue Invoker::_mafia_event(const GameValue& left_arg_, const GameValue& right_arg_)
{
    return controller()->get_mission_events()->rv_event(left_arg_, right_arg_);
}

mafia::game_types::GameValue Invoker::_mafia_signal(const GameValue& left_arg_, const GameValue& right_arg_)
{
    return controller()->rv_signal(left_arg_[0], left_arg_[1], right_arg_);
}

mafia::game_types::GameValue Invoker::_mafia_registerTypes(const GameValue& left_arg_)
{
    using GameDataType = game_types::GameDataType;
    auto invoker = controller()->get_invoker();
    auto loader = controller()->get_loader();
    auto regInfo = loader->get_register_sqf_info();

    //log::debug("Registration Hook Function Called: {}", invoker->_registration_type);
    auto step = invoker->_registration_type;
    invoker->_sqf_game_state = regInfo._gameState;
    Invoker::sqf_game_state = reinterpret_cast<GameState*>(regInfo._gameState);

    GameValue::__vptr_def = left_arg_.get_vtable();
    invoker->type_structures["GV"sv] = {GameValue::__vptr_def, GameValue::__vptr_def};
    Ref<GameData> gd_ar(
            regInfo._types[static_cast<size_t>(GameDataType::ARRAY)]->_createFunction(
                    nullptr
            ));
    std::pair<ValueType_t, ValueType_t> structure = {gd_ar->get_vtable(), gd_ar->get_secondary_vtable()};
    invoker->type_map[structure.first] = "ARRAY"sv;
    invoker->type_structures["ARRAY"sv] = structure;
    game_data::Array::type_def = structure.first;
    game_data::Array::data_type_def = structure.second;
    game_data::Array::pool_alloc_base = loader->_allocator
                                              ._poolAllocs[static_cast<size_t>(game_types::GameDataType::ARRAY)];
    Ref<GameData> gd_sc(regInfo._types[static_cast<size_t>(GameDataType::SCALAR)]->_createFunction(nullptr));
    structure = {gd_sc->get_vtable(), gd_sc->get_secondary_vtable()};
    invoker->type_map[structure.first] = "SCALAR"sv;
    invoker->type_structures["SCALAR"sv] = structure;
    game_data::Number::type_def = structure.first;
    game_data::Number::data_type_def = structure.second;
    game_data::Number::pool_alloc_base = loader->_allocator
                                               ._poolAllocs[static_cast<size_t>(game_types::GameDataType::SCALAR)];
    Ref<GameData> gd_bo(regInfo._types[static_cast<size_t>(GameDataType::BOOL)]->_createFunction(nullptr));
    structure = {gd_bo->get_vtable(), gd_bo->get_secondary_vtable()};
    invoker->type_map[structure.first] = "BOOL"sv;
    invoker->type_structures["BOOL"sv] = structure;
    game_data::Bool::type_def = structure.first;
    game_data::Bool::data_type_def = structure.second;
    game_data::Bool::pool_alloc_base = loader->_allocator
                                             ._poolAllocs[static_cast<size_t>(game_types::GameDataType::BOOL)];
    Ref<GameData> gd_st(regInfo._types[static_cast<size_t>(GameDataType::STRING)]->_createFunction(nullptr));
    structure = {gd_st->get_vtable(), gd_st->get_secondary_vtable()};
    invoker->type_map[structure.first] = "STRING"sv;
    invoker->type_structures["STRING"sv] = structure;
    game_data::String::type_def = structure.first;
    game_data::String::data_type_def = structure.second;
    game_data::String::pool_alloc_base = loader->_allocator
                                               ._poolAllocs[static_cast<size_t>(game_types::GameDataType::STRING)];
    Ref<GameData> gd_code(regInfo._types[static_cast<size_t>(GameDataType::CODE)]->_createFunction(nullptr));
    structure = {gd_code->get_vtable(), gd_code->get_secondary_vtable()};
    invoker->type_map[structure.first] = "CODE"sv;
    invoker->type_structures["CODE"sv] = structure;
    game_data::Code::type_def = structure.first;
    game_data::Code::data_type_def = structure.second;
    game_data::Code::pool_alloc_base = loader->_allocator
                                             ._poolAllocs[static_cast<size_t>(game_types::GameDataType::CODE)];
    Ref<GameData> gd_ob(regInfo._types[static_cast<size_t>(GameDataType::OBJECT)]->_createFunction(nullptr));
    structure = {gd_ob->get_vtable(), gd_ob->get_secondary_vtable()};
    invoker->type_map[structure.first] = "OBJECT"sv;
    invoker->type_structures["OBJECT"sv] = structure;
    game_data::Object::type_def = structure.first;
    game_data::Object::data_type_def = structure.second;
    Ref<GameData> gd_gr(regInfo._types[static_cast<size_t>(GameDataType::GROUP)]->_createFunction(nullptr));
    structure = {gd_gr->get_vtable(), gd_gr->get_secondary_vtable()};
    invoker->type_map[structure.first] = "GROUP"sv;
    invoker->type_structures["GROUP"sv] = structure;
    game_data::Group::type_def = structure.first;
    game_data::Group::data_type_def = structure.second;
    Ref<GameData> gd_conf(regInfo._types[static_cast<size_t>(GameDataType::CONFIG)]->_createFunction(nullptr));
    structure = {gd_conf->get_vtable(), gd_conf->get_secondary_vtable()};
    invoker->type_map[structure.first] = "CONFIG"sv;
    invoker->type_structures["CONFIG"sv] = structure;
    game_data::Config::type_def = structure.first;
    game_data::Config::data_type_def = structure.second;
    Ref<GameData> gd_cont(regInfo._types[static_cast<size_t>(GameDataType::CONTROL)]->_createFunction(nullptr));
    structure = {gd_cont->get_vtable(), gd_cont->get_secondary_vtable()};
    invoker->type_map[structure.first] = "CONTROL"sv;
    invoker->type_structures["CONTROL"sv] = structure;
    game_data::Control::type_def = structure.first;
    game_data::Control::data_type_def = structure.second;
    Ref<GameData> gd_di(regInfo._types[static_cast<size_t>(GameDataType::DISPLAY)]->_createFunction(nullptr));
    structure = {gd_di->get_vtable(), gd_di->get_secondary_vtable()};
    invoker->type_map[structure.first] = "DISPLAY"sv;
    invoker->type_structures["DISPLAY"sv] = structure;
    game_data::Display::type_def = structure.first;
    game_data::Display::data_type_def = structure.second;
    Ref<GameData> gd_loc(regInfo._types[static_cast<size_t>(GameDataType::LOCATION)]->_createFunction(nullptr));
    structure = {gd_loc->get_vtable(), gd_loc->get_secondary_vtable()};
    invoker->type_map[structure.first] = "LOCATION"sv;
    invoker->type_structures["LOCATION"sv] = structure;
    game_data::Location::type_def = structure.first;
    game_data::Location::data_type_def = structure.second;
    Ref<GameData> gd_scr(regInfo._types[static_cast<size_t>(GameDataType::SCRIPT)]->_createFunction(nullptr));
    structure = {gd_scr->get_vtable(), gd_scr->get_secondary_vtable()};
    invoker->type_map[structure.first] = "SCRIPT"sv;
    invoker->type_structures["SCRIPT"sv] = structure;
    game_data::Script::type_def = structure.first;
    game_data::Script::data_type_def = structure.second;
    Ref<GameData> gd_si(regInfo._types[static_cast<size_t>(GameDataType::SIDE)]->_createFunction(nullptr));
    structure = {gd_si->get_vtable(), gd_si->get_secondary_vtable()};
    invoker->type_map[structure.first] = "SIDE"sv;
    invoker->type_structures["SIDE"sv] = structure;
    game_data::Side::type_def = structure.first;
    game_data::Side::data_type_def = structure.second;
    Ref<GameData> gd_te(regInfo._types[static_cast<size_t>(GameDataType::TEXT)]->_createFunction(nullptr));
    structure = {gd_te->get_vtable(), gd_te->get_secondary_vtable()};
    invoker->type_map[structure.first] = "TEXT"sv;
    invoker->type_structures["TEXT"sv] = structure;
    game_data::RVText::type_def = structure.first;
    game_data::RVText::data_type_def = structure.second;
    Ref<GameData> gd_tm(regInfo._types[static_cast<size_t>(GameDataType::TEAM_MEMBER)]->_createFunction(nullptr));
    structure = {gd_tm->get_vtable(), gd_tm->get_secondary_vtable()};
    invoker->type_map[structure.first] = "TEAM_MEMBER"sv;
    invoker->type_structures["TEAM_MEMBER"sv] = structure;
    game_data::TeamMember::type_def = structure.first;
    game_data::TeamMember::data_type_def = structure.second;


    Ref<GameData> gd_nt(regInfo._types[static_cast<size_t>(GameDataType::NOTHING)]->_createFunction(nullptr));
    structure = {gd_nt->get_vtable(), gd_nt->get_secondary_vtable()};
    invoker->type_map[structure.first] = "NOTHING"sv;
    invoker->type_structures["NOTHING"sv] = structure;
    game_data::Nothing::type_def = structure.first;
    game_data::Nothing::data_type_def = structure.second;
    //#TODO add nothing and Nil

    structure = {left_arg_.data->get_vtable(), left_arg_.data->get_secondary_vtable()};
    invoker->type_map[structure.first] = "NAMESPACE"sv;
    invoker->type_structures["NAMESPACE"sv] = structure;
    game_data::Namespace::type_def = structure.first;
    game_data::Namespace::data_type_def = structure.second;

    SQFScriptType::type_def = loader->get_register_sqf_info()._type_vtable;
    structure = {SQFScriptType::type_def, SQFScriptType::type_def};
    invoker->type_map[structure.first] = "SQF_SCRIPT_TYPE"sv;
    invoker->type_structures["SQF_SCRIPT_TYPE"sv] = structure;

    //log::debug("invoker::_mafia_registerTypes done");
    //log::flush();
    return true;
}
