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

#include "sqf_functions.h"
#include "mafia.h"
#include "rv_controller.h"
#include "loader.h"
#include "game_types/game_value.h"
#include "game_types/game_data_type.h"

using namespace mafia;
using namespace mafia::_private;
using namespace mafia::game_types;
using namespace std::literals::string_view_literals;

RegisteredSQFFunctionWrapper::RegisteredSQFFunctionWrapper(GameDataType return_type_, gsNular* func_):
        _type(functionType::sqf_nular), _name(func_->_name2),
        _nular(func_), _lArgType(GameDataType::NOTHING), _rArgType(GameDataType::NOTHING), _returnType(return_type_) {}

RegisteredSQFFunctionWrapper::RegisteredSQFFunctionWrapper(
        GameDataType return_type_, GameDataType left_arg_type_,
        gsFunction* func_
):
        _type(functionType::sqf_function), _name(func_->_name2),
        _func(func_), _lArgType(left_arg_type_), _rArgType(GameDataType::NOTHING), _returnType(return_type_) {}

RegisteredSQFFunctionWrapper::RegisteredSQFFunctionWrapper(
        GameDataType return_type_, GameDataType left_arg_type_,
        GameDataType right_arg_type_, gsOperator* func_
):
        _type(functionType::sqf_operator), _name(func_->_name2),
        _op(func_), _lArgType(left_arg_type_), _rArgType(right_arg_type_), _returnType(return_type_) {}

RegisteredSQFFunctionWrapper::RegisteredSQFFunctionWrapper(GameDataType return_type_, gsNular* func_, undo_info undo_):
        _type(functionType::sqf_nular), _name(func_->_name2),
        _nular(func_), _lArgType(GameDataType::NOTHING), _rArgType(GameDataType::NOTHING), _returnType(return_type_),
        undo(std::make_unique<undo_info>(undo_)) {}

RegisteredSQFFunctionWrapper::RegisteredSQFFunctionWrapper(
        GameDataType return_type_, GameDataType left_arg_type_,
        gsFunction* func_, undo_info undo_
):
        _type(functionType::sqf_function), _name(func_->_name2),
        _func(func_), _lArgType(left_arg_type_), _rArgType(GameDataType::NOTHING), _returnType(return_type_),
        undo(std::make_unique<undo_info>(undo_)) {}

RegisteredSQFFunctionWrapper::RegisteredSQFFunctionWrapper(
        GameDataType return_type_, GameDataType left_arg_type_,
        GameDataType right_arg_type_, gsOperator* func_,
        undo_info undo_
):
        _type(functionType::sqf_operator), _name(func_->_name2),
        _op(func_), _lArgType(left_arg_type_), _rArgType(right_arg_type_), _returnType(return_type_),
        undo(std::make_unique<undo_info>(undo_)) {}


template<GameDataType returnType>
class unusedSQFFunction
{
public:
    static GameValue* CDECL unusedNular(GameValue* ret_, uintptr_t gs_)
    {
        switch (returnType)
        {
            case GameDataType::SCALAR:::new(ret_) GameValue("unimplemented"sv);
                break;
            case GameDataType::BOOL:::new(ret_) GameValue(false);
                break;
            case GameDataType::ARRAY:::new(ret_) GameValue(std::vector<GameValue>());
                break;
            case GameDataType::STRING:::new(ret_) GameValue("unimplemented"sv);
                break;
            default:::new(ret_) GameValue();
                break;
        }
        return ret_;
    }

    static GameValue* CDECL unusedFunction(GameValue* ret_, uintptr_t gs_, uintptr_t larg_)
    {
        return unusedNular(ret_, gs_);
    }

    static GameValue* CDECL unusedOperator(GameValue* ret_, uintptr_t gs_, uintptr_t larg_, uintptr_t rarg_)
    {
        return unusedNular(ret_, gs_);
    }
};
#define UNUSED_FUNC_SWITCH_FOR_GAMETYPES(ptr, type) \
switch (_returnType) { \
    case GameDataType::SCALAR: \
        *ptr->_operator->procedure_addr = unusedSQFFunction<GameDataType::SCALAR>::type; \
    break; \
    case GameDataType::BOOL: \
        *ptr->_operator->procedure_addr = unusedSQFFunction<GameDataType::BOOL>::type; \
        break; \
    case GameDataType::ARRAY: \
        *ptr->_operator->procedure_addr = unusedSQFFunction<GameDataType::ARRAY>::type; \
        break; \
    case GameDataType::STRING: \
        *ptr->_operator->procedure_addr = unusedSQFFunction<GameDataType::STRING>::type; \
        break; \
    case GameDataType::OBJECT: \
        *ptr->_operator->procedure_addr = unusedSQFFunction<GameDataType::OBJECT>::type; \
        break; \
}

void RegisteredSQFFunctionWrapper::setUnused() noexcept
{
    //switch (_type) {
    //    case functionType::sqf_nular:
    //        if (_nular && _nular->_operator)
    //            UNUSED_FUNC_SWITCH_FOR_GAMETYPES(_nular, unusedNular);
    //        break;
    //    case functionType::sqf_function:
    //        if (_func && _func->_operator)
    //            UNUSED_FUNC_SWITCH_FOR_GAMETYPES(_func, unusedFunction);
    //        break;
    //    case functionType::sqf_operator:
    //        if (_op && _op->_operator)
    //            UNUSED_FUNC_SWITCH_FOR_GAMETYPES(_op, unusedOperator);
    //        break;
    //}
}

mafia::RegisteredSQFFunctionImpl::RegisteredSQFFunctionImpl(
        std::shared_ptr<RegisteredSQFFunctionWrapper> func_
) noexcept: _func(func_)
{

}

mafia::RegisteredSQFFunctionImpl::~RegisteredSQFFunctionImpl()
{
    if (controller()->get_sqf_functions()->_canRegister)
    {
        if (controller()->get_sqf_functions()->unregister_sqf_function(_func))
        {
            return;
        }
    }
    _func->setUnused();
}

mafia::SQFFunctions::SQFFunctions() noexcept = default;

mafia::SQFFunctions::~SQFFunctions() = default;

void mafia::SQFFunctions::initialize() noexcept
{
    _registerFuncs = mafia::controller()->get_loader()->get_register_sqf_info();
    _canRegister = true;
}

void SQFFunctions::setDisabled() noexcept
{
    _canRegister = false;
}

RegisteredSQFFunction mafia::SQFFunctions::register_sqf_function(
        std::string_view name,
        std::string_view description,
        WrapperFunctionBinary function_,
        GameDataType return_arg_type,
        GameDataType left_arg_type,
        GameDataType right_arg_type
)
{
    //Core plugins can overwrite existing functions. Which is "safe". So they can pass along for now.
    if (!_canRegister/* && mafia::cert::current_security_class != cert::signing::security_class::core*/)
    {
        throw std::logic_error("Can only register SQF Commands on preStart");
    }
    if (name.length() > 256)
    {
        throw std::length_error("mafia::sqf_functions::register_sqf_function name can maximum be 256 chars long");
    }

    SQFScriptType retType {
            _registerFuncs._type_vtable, _registerFuncs._types[static_cast<size_t>(return_arg_type)], nullptr
    };
    SQFScriptType leftType {
            _registerFuncs._type_vtable, _registerFuncs._types[static_cast<size_t>(left_arg_type)], nullptr
    };
    SQFScriptType rightype {
            _registerFuncs._type_vtable, _registerFuncs._types[static_cast<size_t>(right_arg_type)], nullptr
    };


    std::string lowerName(name);
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    const auto test = findBinary("getvariable", GameDataType::OBJECT, GameDataType::ARRAY);

    auto operators = findOperators(std::string(name));
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);

    if (!operators)
    {
        auto table = gs->_scriptOperators.get_table_for_key(lowerName.c_str());
        auto found = _keeper.find(reinterpret_cast<uintptr_t>(table->data()));
        if (found == _keeper.end())
        {
            //copy array and keep old one active so we never deallocate it and pointers into the old array stay valid
            containers::AutoArray<GameOperators> backup(table->begin(), table->end());
            _keeper.insert_or_assign(
                    reinterpret_cast<uintptr_t>(table->data()),
                    std::move(*reinterpret_cast<containers::AutoArray<char>*>(table)));
            *table = std::move(backup);
        }

        operators = static_cast<GameOperators*>(table->push_back(GameOperators(String(lowerName))));
        operators->copyPH(test);
    }
    else
    {  //Name already exists


        if (auto found = findBinary(std::string(name), left_arg_type, right_arg_type); found)
        {//Function with same arg types already exists
            //if (mafia::cert::current_security_class != cert::signing::security_class::core)
            //    return registered_sqf_function {nullptr}; //Core certified plugins have exception for this rule

            //We only manipulate elements that are resolved at runtime.
            //This redirects a normal script command to Intercept. You can still call the original from within Intercept.

            RegisteredSQFFunctionWrapper::undo_info undo;
            undo._procB = found->_operator->procedure_addr;
            found->_operator->procedure_addr = reinterpret_cast<BinaryFunction*>(function_);
#ifndef __linux__
            undo._description = found->_description;
            found->_description = description;
#endif

            /*LOG(INFO, "sqf_functions::register_sqf_function binary OVERRIDE {} {} = {:x} {} = {:x} {} = {:x} @ {:x}", name,
                types::_private::to_string(return_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(return_arg_type)]),
                types::_private::to_string(left_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(left_arg_type)]),
                types::_private::to_string(right_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(right_arg_type)]),
                reinterpret_cast<uintptr_t>(found)
            );*/
            auto wrapper = std::make_shared<RegisteredSQFFunctionWrapper>(
                    return_arg_type, left_arg_type, right_arg_type,
                    found
            );

            return RegisteredSQFFunction(std::make_shared<RegisteredSQFFunctionImpl>(wrapper));
        }
        if (!_canRegister)
        { throw std::logic_error("Can only register SQF Commands on preStart"); }
    }

    _private::gsOperator op;
    op._name = name;
    op._name2 = lowerName;
#ifndef __linux__
    op._description = description;
#endif
    op.copyPH(test);
    op._operator = RVAllocator<BinaryOperator>::create_single();
    op._operator->_vtable = test->_operator->_vtable;
    op._operator->procedure_addr = reinterpret_cast<BinaryFunction*>(function_);
    op._operator->return_type = retType;
    op._operator->arg1_type = leftType;
    op._operator->arg2_type = rightype;

    auto inserted = static_cast<_private::gsOperator*>(operators->push_back(op));


    //insertBinary(_registerFuncs._gameState, op);

    //auto inserted = findBinary(name, left_arg_type, right_arg_type);
    //std::stringstream stream;
    /*LOG(INFO, "sqf_functions::register_sqf_function binary {} {} = {:x} {} = {:x} {} = {:x} @ {:x}", name,
        types::_private::to_string(return_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(return_arg_type)]),
        types::_private::to_string(left_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(left_arg_type)]),
        types::_private::to_string(right_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(right_arg_type)]),
        reinterpret_cast<uintptr_t>(inserted)
    );*/

    auto wrapper = std::make_shared<RegisteredSQFFunctionWrapper>(
            return_arg_type, left_arg_type, right_arg_type,
            inserted
    );

    return RegisteredSQFFunction(std::make_shared<RegisteredSQFFunctionImpl>(wrapper));
}

RegisteredSQFFunction mafia::SQFFunctions::register_sqf_function(
        std::string_view name,
        std::string_view description,
        WrapperFunctionUnary function_,
        GameDataType return_arg_type,
        GameDataType right_arg_type
)
{
    //Core plugins can overwrite existing functions. Which is "safe". So they can pass along for now.
    if (!_canRegister/* && mafia::cert::current_security_class != cert::signing::security_class::core*/)
    {
        throw std::logic_error("Can only register SQF Commands on preStart");
    }
    if (name.length() > 256)
    {
        throw std::length_error("mafia::sqf_functions::register_sqf_function name can maximum be 256 chars long");
    }

    SQFScriptType retType {
            _registerFuncs._type_vtable, _registerFuncs._types[static_cast<size_t>(return_arg_type)], nullptr
    };
    SQFScriptType rightype {
            _registerFuncs._type_vtable, _registerFuncs._types[static_cast<size_t>(right_arg_type)], nullptr
    };


    const auto test = findUnary("diag_log", GameDataType::ANY);
    std::string lowerName(name);
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    auto functions = findFunctions(std::string(name));
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);

    if (!functions)
    {
        if (!_canRegister)
        { throw std::logic_error("Can only register SQF Commands on preStart"); }
        auto table = gs->_scriptFunctions.get_table_for_key(lowerName.c_str());
        auto found = _keeper.find(reinterpret_cast<uintptr_t>(table->data()));
        if (found == _keeper.end())
        {
            //copy array and keep old one active so we never deallocate it and pointers into the old array stay valid
            containers::AutoArray<GameFunctions> backup(table->begin(), table->end());
            _keeper.insert_or_assign(
                    reinterpret_cast<uintptr_t>(table->data()),
                    std::move(*reinterpret_cast<containers::AutoArray<char>*>(table)));
            *table = std::move(backup);
        }

        functions = static_cast<GameFunctions*>(table->push_back(GameFunctions(String(lowerName))));
        functions->copyPH(test);
    }
    else
    { //Name already exists
        if (auto found = findUnary(std::string(name), right_arg_type); found)
        {//Function with same arg types already exists
            //if (mafia::cert::current_security_class != cert::signing::security_class::core)
            //    return registered_sqf_function {nullptr}; //Core certified plugins have exception for this rule

            //We only manipulate elements that are resolved at runtime.
            //This redirects a normal script command to Intercept. You can still call the original from within Intercept.

            RegisteredSQFFunctionWrapper::undo_info undo;
            undo._procU = found->_operator->procedure_addr;
            found->_operator->procedure_addr = reinterpret_cast<UnaryFunction*>(function_);
#ifndef __linux__
            undo._description = found->_description;
            found->_description = description;
#endif

            /*LOG(INFO, "sqf_functions::register_sqf_function unary OVERRIDE {} {} = {:x} {} = {:x} @ {:x}", name,
                types::_private::to_string(return_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(return_arg_type)]),
                types::_private::to_string(right_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(right_arg_type)]),
                reinterpret_cast<uintptr_t>(found)
            );*/

            auto wrapper = std::make_shared<RegisteredSQFFunctionWrapper>(return_arg_type, right_arg_type, found, undo);

            return RegisteredSQFFunction(std::make_shared<RegisteredSQFFunctionImpl>(wrapper));
        }
        if (!_canRegister)
        { throw std::logic_error("Can only register SQF Commands on preStart"); }
    }

    _private::gsFunction op;
    op._name = name;
    op._name2 = lowerName;
#ifndef __linux__
    op._description = description;
#endif
    op.copyPH(test);
    op._operator = RVAllocator<UnaryOperator>::create_single();
    op._operator->_vtable = test->_operator->_vtable;
    op._operator->procedure_addr = reinterpret_cast<UnaryFunction*>(function_);
    op._operator->return_type = retType;
    op._operator->arg_type = rightype;

    auto inserted = static_cast<_private::gsFunction*>(functions->push_back(op));


    //auto inserted = findUnary(name, right_arg_type); //Could use this to check if == ref returned by push_back.. But I'm just assuming it works right now
    //std::stringstream stream;
    /*LOG(INFO, "sqf_functions::register_sqf_function unary {} {} = {:x} {} = {:x} @ {:x}", name,
        types::_private::to_string(return_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(return_arg_type)]),
        types::_private::to_string(right_arg_type), reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(right_arg_type)]),
        reinterpret_cast<uintptr_t>(inserted)
    );*/

    auto wrapper = std::make_shared<RegisteredSQFFunctionWrapper>(return_arg_type, right_arg_type, inserted);

    return RegisteredSQFFunction(std::make_shared<RegisteredSQFFunctionImpl>(wrapper));
}

RegisteredSQFFunction mafia::SQFFunctions::register_sqf_function(
        std::string_view name,
        std::string_view description,
        WrapperFunctionNular function_,
        GameDataType return_arg_type
)
{
    //Core plugins can overwrite existing functions. Which is "safe". So they can pass along for now.
    if (!_canRegister/* && mafia::cert::current_security_class != cert::signing::security_class::core*/)
    {
        throw std::logic_error("Can only register SQF Commands on preStart");
    }
    if (name.length() > 256)
    {
        throw std::length_error("mafia::sqf_functions::register_sqf_function name can maximum be 256 chars long");
    }

    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);


    SQFScriptType retType {
            _registerFuncs._type_vtable, _registerFuncs._types[static_cast<size_t>(return_arg_type)], nullptr
    };

    const auto test = findNular("player");
    std::string lowerName(name);
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    const auto alreadyExists = findNular(std::string(name));

    if (alreadyExists)
    {//Name already exists
        //if (mafia::cert::current_security_class != cert::signing::security_class::core)
        //    return registered_sqf_function {
        //            nullptr
        //    }; //Core certified plugins have exception for this rule
        //We only manipulate elements that are resolved at runtime.
        //This redirects a normal script command to Intercept. You can still call the original from within Intercept.

        RegisteredSQFFunctionWrapper::undo_info undo;
        undo._procN = alreadyExists->_operator->procedure_addr;
        alreadyExists->_operator->procedure_addr = reinterpret_cast<NularFunction*>(function_);
#ifndef __linux__
        undo._description = alreadyExists->_description;
        alreadyExists->_description = description;
#endif
        //LOG(INFO, "sqf_functions::register_sqf_function nular OVERRIDE {} {} = {:x} @ {:x}", name, types::_private::to_string(return_arg_type)
        //    , reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(return_arg_type)]), reinterpret_cast<uintptr_t>(alreadyExists)
        //);
        auto wrapper = std::make_shared<RegisteredSQFFunctionWrapper>(return_arg_type, alreadyExists, undo);

        return RegisteredSQFFunction(std::make_shared<RegisteredSQFFunctionImpl>(wrapper));
    }

    if (!_canRegister)
    { throw std::logic_error("Can only register SQF Commands on preStart"); }

    _private::gsNular op;
    op._name = name;
    op._name2 = lowerName; //#TODO move this into a constructor. for all types
#ifndef __linux__
    op._description = description;
#endif
    op.copyPH(test);
    op._operator = RVAllocator<NularOperator>::create_single();
    op._operator->_vtable = test->_operator->_vtable;
    op._operator->procedure_addr = reinterpret_cast<NularFunction*>(function_);
    op._operator->return_type = retType;
#ifndef __linux__
    op._category = "mafia"sv;
#endif

    auto table = gs->_scriptNulars.get_table_for_key(lowerName.c_str());
    auto found = _keeper.find(reinterpret_cast<uintptr_t>(table->data()));
    if (found == _keeper.end())
    {
        //copy array and keep old one active so we never deallocate it and pointers into the old array stay valid
        containers::AutoArray<gsNular> backup(table->begin(), table->end());
        _keeper.insert_or_assign(
                reinterpret_cast<uintptr_t>(table->data()),
                std::move(*reinterpret_cast<containers::AutoArray<char>*>(table)));
        *table = std::move(backup);
    }

    auto inserted = static_cast<_private::gsNular*>(table->push_back(op));

    //auto inserted = findNular(name);  Could use this to confirm that inserted points to correct value
    //std::stringstream stream;
    //LOG(INFO, "sqf_functions::register_sqf_function nular {} {} = {:x} @ {:x}", name, types::_private::to_string(return_arg_type)
    //    , reinterpret_cast<uintptr_t>(_registerFuncs._types[static_cast<size_t>(return_arg_type)]), reinterpret_cast<uintptr_t>(inserted)
    //);
    auto wrapper = std::make_shared<RegisteredSQFFunctionWrapper>(return_arg_type, inserted);

    return RegisteredSQFFunction(std::make_shared<RegisteredSQFFunctionImpl>(wrapper));
}

bool SQFFunctions::unregister_sqf_function(const std::shared_ptr<RegisteredSQFFunctionWrapper>& shared)
{
    //Undoing a override is "safe"
    if (!_canRegister && !shared->undo)
    { throw std::runtime_error("Can only unregister SQF Commands on preStart"); }

    //Handle undo's
    if (shared->undo)
    {
        switch (shared->_type)
        {
            case functionType::sqf_nular:shared->_nular->_operator->procedure_addr = shared->undo->_procN;
#ifndef __linux__
                shared->_nular->_description = shared->undo->_description;
#endif
                return true;
            case functionType::sqf_function:shared->_func->_operator->procedure_addr = shared->undo->_procU;
#ifndef __linux__
                shared->_func->_description = shared->undo->_description;
#endif
                return true;
            case functionType::sqf_operator:shared->_op->_operator->procedure_addr = shared->undo->_procB;
#ifndef __linux__
                shared->_op->_description = shared->undo->_description;
#endif
                return true;
        }
        return false;
    }

    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);
    switch (shared->_type)
    {
        case functionType::sqf_nular:
        {
            auto table = gs->_scriptNulars.get_table_for_key(shared->_name.c_str());

            auto found = std::find_if(
                    table->begin(), table->end(), [name = shared->_name](const gsNular& fnc) {
                        return fnc._name2 == name;
                    }
            );
            if (found != table->end())
            {
                table->erase(found);
                return true;
            }
            return false;
        }
            break;
        case functionType::sqf_function:
        {
            auto& table = gs->_scriptFunctions.get(shared->_name.c_str());
            auto found = std::find_if(
                    table.begin(), table.end(), [name = shared->_name](const gsFunction& fnc) {
                        return fnc._name2 == name;
                    }
            );
            if (found != table.end())
            {
                table.erase(found);
                return true;
            }
            return false;
        }
            break;
        case functionType::sqf_operator:
        {
            auto& table = gs->_scriptOperators.get(shared->_name.c_str());
            auto found = std::find_if(
                    table.begin(), table.end(), [name = shared->_name](const gsOperator& fnc) {
                        return fnc._name2 == name;
                    }
            );
            if (found != table.end())
            {
                table.erase(found);
                return true;
            }
            return false;
        }
            break;
    }
    return false;
}


std::pair<GameDataType, SQFScriptType> mafia::SQFFunctions::register_sqf_type(
        std::string_view name,
        std::string_view localizedName,
        std::string_view description, std::string_view typeName,
        ScriptTypeInfo::createFunc cf
)
{
    if (!_canRegister)
    { throw std::runtime_error("Can only register SQF Types on preStart"); }
    if (name.length() > 128)
    {
        throw std::length_error("mafia::sqf_functions::register_sqf_type name can maximum be 128 chars long");
    }
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);

    auto newType = RVAllocator<ScriptTypeInfo>::create_single(
#ifdef __linux__
            name,cf,localizedName,localizedName
#else
            name, cf, localizedName, localizedName, description, String("mafia"sv), typeName
#endif
    );
    gs->_scriptTypes.emplace_back(newType);
    const auto newIndex = _registerFuncs._types.size();
    _registerFuncs._types.emplace_back(newType);
    //LOG(INFO, "sqf_functions::register_sqf_type {} {} {} ", name, localizedName, description, typeName);
    game_types::_private::add_game_data_type(name, static_cast<GameDataType>(newIndex));
    return {static_cast<GameDataType>(newIndex), {_registerFuncs._type_vtable, newType, nullptr}};
}

SQFScriptType SQFFunctions::register_compound_sqf_type(containers::AutoArray<GameDataType> types)
{
    if (!_canRegister)
    { throw std::runtime_error("Can only register SQF Types on preStart"); }
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);

    containers::AutoArray<const ScriptTypeInfo*> resolvedTypes;

    for (auto& it : types)
    {
        auto argTypeString = mafia::game_types::to_string(it);
        for (auto& type : gs->get_script_types())
        {
            if (type->_name == argTypeString)
            {
                resolvedTypes.emplace_back(type);
            }
        }
    }

    auto newType = RVAllocator<CompoundScriptTypeInfo>::create_single(
            resolvedTypes
    );

    return {_registerFuncs._type_vtable, nullptr, newType};
}

mafia::_private::gsNular* mafia::SQFFunctions::findNular(std::string name) const
{
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    auto& found = gs->_scriptNulars.get(name.c_str());
    if (gs->_scriptNulars.is_null(found))
    { return nullptr; }
    return &found;
}

mafia::_private::gsFunction* mafia::SQFFunctions::findUnary(std::string name, GameDataType argument_type) const
{
    //gs->_scriptFunctions.get_table_for_key(name.c_str())->for_each([](const game_functions& it) {
    //    OutputDebugStringA(it._name.c_str());
    //    OutputDebugStringA("\n");
    //});
    auto funcs = findFunctions(name);
    if (!funcs)
    { return nullptr; }
    auto argTypeString = mafia::game_types::to_string(argument_type);
    for (auto& it : *funcs)
    {
        auto types = it._operator->arg_type.type();
        if (types.find(argTypeString) != types.end())
        {
            return &it;
        }
    }
    return nullptr;
}

mafia::_private::gsOperator* mafia::SQFFunctions::findBinary(
        std::string name,
        GameDataType left_argument_type,
        GameDataType right_argument_type
) const
{
    //gs->_scriptOperators.get_table_for_key(name.c_str())->for_each([](const game_operators& it) {
    //    OutputDebugStringA(it._name.c_str());
    //    OutputDebugStringA("\n");
    //});

    auto operators = findOperators(name);
    if (!operators)
    { return nullptr; }
    auto left_argTypeString = mafia::game_types::to_string(left_argument_type);
    auto right_argTypeString = mafia::game_types::to_string(right_argument_type);
    for (auto& it : *operators)
    {
        auto left_types = it._operator->arg1_type.type();
        if (left_types.find(left_argTypeString) != left_types.end())
        {
            auto right_types = it._operator->arg2_type.type();
            if (right_types.find(right_argTypeString) != right_types.end())
            {
                return &it;
            }
        }
    }
    return nullptr;
}

mafia::_private::GameOperators* mafia::SQFFunctions::findOperators(std::string name) const
{
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    auto& found = gs->_scriptOperators.get(name.c_str());
    if (gs->_scriptOperators.is_null(found))
    { return nullptr; }
    return &found;
}

mafia::_private::GameFunctions* mafia::SQFFunctions::findFunctions(std::string name) const
{
    auto gs = reinterpret_cast<GameState*>(_registerFuncs._gameState);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    auto& found = gs->_scriptFunctions.get(name.c_str());
    if (gs->_scriptFunctions.is_null(found))
    { return nullptr; }
    return &found;
}
