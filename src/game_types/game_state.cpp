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
 * File created by Anthony Ilareguy on 21/03/2021.
 * [File Description]
 *
 ********************************************************
 * 
 * [Credits]
 *
 ********************************************************/

#include "game_state.h"
#include "game_var_space.h"
#include "vm_context.h"
#include "script_type_info.h"
#include "sqf_script_type.h"

using namespace mafia::game_types;

GameState::GameEvaluator::GameEvaluator(GameVarSpace* var)
{
    local = var;
    static int handleG = 0;
    handle = handleG++;
    _2 = false;
}

void GameState::GameEvaluator::operator delete(void* ptr_, std::size_t)
{
    RVAllocator<GameEvaluator>::destroy_deallocate(static_cast<GameEvaluator*>(ptr_));
}

mafia::Ref<game_data::Namespace> GameState::get_current_namespace(GameState::namespace_type type) const
{
    return varspace;
}

mafia::Ref<game_data::Namespace> GameState::get_global_namespace(GameState::namespace_type type) const
{
    return namespaces[static_cast<int>(type)];
}

GameValue GameState::get_local_variable(std::string_view name) const
{
    if (!eval || !eval->local)
    { return {}; }
    auto var = eval->local->get_variable(name);
    if (!var)
    { return {}; }
    auto r = GameValue();
    r.copy(var->value);
    return std::move(r);
}

void GameState::set_local_variable(const String& name, GameValue value, bool editExisting) const
{
    if (!eval || !eval->local)
    { return; }
    if (editExisting)
    {
        auto var = eval->local->get_variable(name);
        if (var)
        {
            var->value = std::move(value);
            return;
        }
    }
    eval->local->set_variable(name, std::move(value));
}

void GameState::delete_local_variable(std::string_view name)
{
    if (!eval || !eval->local)
    { return; }
    eval->local->delete_variable(name);
}

void GameState::set_script_error(GameState::GameEvaluator::evaluator_error_type type, String message)
{
    if (!eval)
    { return; } //Don't know why or how this could happen, but better safe than sorry.
    eval->_errorType = type;
    eval->_errorMessage = message;
    if (current_context)
    {
        eval->_errorPosition = current_context->sdocpos;
    }
}

void GameState::set_script_error(
        GameState::GameEvaluator::evaluator_error_type type,
        String message, SourceDocPosition position
)
{
    if (!eval)
    { return; } //Don't know why or how this could happen, but better safe than sorry.
    eval->_errorType = type;
    eval->_errorMessage = message;
    eval->_errorPosition = position;
}

bool GameState::error_check_size(GameValue value, size_t min_size)
{
    if (value.size() >= min_size)
    { return true; }
    auto message =
            std::to_string(value.size()) + " elements provided, " + std::to_string(min_size) + " expected";

    set_script_error(GameEvaluator::evaluator_error_type::dim, static_cast<String>(message));
    return false;
}

bool GameState::error_check_type(GameValue value, GameDataType expected_type)
{
    if (value.type_enum() == expected_type)
    { return true; }

    auto actualName = to_string(value.type_enum());
    auto expectedName = to_string(expected_type);
    auto message = String("Type ") + actualName + ", expected " + expectedName;
    set_script_error(GameEvaluator::evaluator_error_type::type, message);
    return false;
}

VMContext* GameState::get_vm_context() const
{
    return current_context;
}

GameState::GameEvaluator* GameState::get_evaluator() const
{
    return eval;
}

const auto& GameState::get_script_types()
{
    return _scriptTypes;
}

const auto& GameState::get_script_functions()
{
    return _scriptFunctions;
}

const auto& GameState::get_script_operators()
{
    return _scriptOperators;
}

const auto& GameState::get_script_nulars()
{
    return _scriptNulars;
}

GameData* GameState::create_game_data_from_type(const SQFScriptType& type, ParamArchive* ar) const
{
    for (auto& it : _scriptTypes)
    {
        if (SQFScriptType(it) == type)
        {
            if (it->_createFunction)
            { return it->_createFunction(ar); }
            return nullptr;
        }
    }
    return nullptr;
}
