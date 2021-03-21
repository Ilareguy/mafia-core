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

#include "game_value.h"

using namespace mafia::game_types;

GameValue::GameValue(float val_)
{
    set_vtable(__vptr_def);
    data = new game_data_number(val_);
}

GameValue::GameValue(int val_): GameValue(static_cast<float>(val_)) {}

GameValue::GameValue(size_t val_): GameValue(static_cast<float>(val_)) {}

GameValue::GameValue(bool val_)
{
    set_vtable(__vptr_def);
    data = new game_data_bool(val_);
}

GameValue::GameValue(const std::string& val_)
{
    set_vtable(__vptr_def);
    data = new game_data_string(val_);
}

GameValue::GameValue(const r_string& val_)
{
    set_vtable(__vptr_def);
    data = new game_data_string(val_);
}

GameValue::GameValue(std::string_view val_)
{
    set_vtable(__vptr_def);
    data = new game_data_string(val_);
}

GameValue::GameValue(const std::vector<GameValue>& list_)
{
    set_vtable(__vptr_def);
    data = new game_data_array(list_);
}

GameValue::GameValue(const std::initializer_list<GameValue>& list_)
{
    set_vtable(__vptr_def);
    data = new game_data_array(list_);
}

GameValue::GameValue(auto_array<GameValue>&& array_)
{
    set_vtable(__vptr_def);
    data = new game_data_array(std::move(array_));
}

GameValue::GameValue(const vector3& vec_)
{
    set_vtable(__vptr_def);
    data = new game_data_array({vec_.x, vec_.y, vec_.z});
}

GameValue::GameValue(const vector2& vec_)
{
    set_vtable(__vptr_def);
    data = new game_data_array({vec_.x, vec_.y});
}

GameValue::GameValue(const internal_object& internal_)
{
    set_vtable(__vptr_def); //object class has bugged vtable :u
    data = internal_.data;
}

GameValue& GameValue::operator=(const GameValue& copy_)
{
    copy(copy_);
    return *this;
}

GameValue& GameValue::operator=(GameValue&& move_) noexcept
{
    if (this == &move_)
    {
        return *this;
    }
    set_vtable(__vptr_def);
    data = move_.data;
    move_.data = nullptr;
    return *this;
}

GameValue& GameValue::operator=(const float val_)
{
    data = new game_data_number(val_);
    return *this;
}

GameValue& GameValue::operator=(bool val_)
{
    data = new game_data_bool(val_);
    return *this;
}

GameValue& GameValue::operator=(const std::string& val_)
{
    data = new game_data_string(val_);
    return *this;
}

GameValue& GameValue::operator=(const r_string& val_)
{
    data = new game_data_string(val_);
    return *this;
}

GameValue& GameValue::operator=(std::string_view val_)
{
    data = new game_data_string(val_);
    return *this;
}

GameValue& GameValue::operator=(const std::vector<GameValue>& list_)
{
    data = new game_data_array(list_);
    return *this;
}

GameValue& GameValue::operator=(const vector3& vec_)
{
    data = new game_data_array({vec_.x, vec_.y, vec_.z});
    return *this;
}

GameValue& GameValue::operator=(const vector2& vec_)
{
    data = new game_data_array({vec_.x, vec_.y});
    return *this;
}

GameValue& GameValue::operator=(const internal_object& internal_)
{
    data = internal_.data;
    set_vtable(__vptr_def);
    return *this;
}

GameValue::operator int() const
{
    if (data)
    {
#ifdef MAFIA_SAFE_CONVERSIONS
        const auto type = data->get_vtable();
            if (data->get_vtable() == game_data_number::type_def)
                throw GameValue_conversion_error("Invalid conversion to scalar");
#endif
        return static_cast<int>(data->get_as_number());
    }

    return 0;
}

GameValue::operator float() const
{
    if (data)
    {
#ifdef MAFIA_SAFE_CONVERSIONS
        const auto type = data->get_vtable();
            if (data->get_vtable() == game_data_number::type_def)
                throw GameValue_conversion_error("Invalid conversion to scalar");
#endif
        return data->get_as_number();
    }
    return 0.f;
}

GameValue::operator bool() const
{
    if (data)
    {
#ifdef MAFIA_SAFE_CONVERSIONS
        const auto type = data->get_vtable();
            if (data->get_vtable() == game_data_bool::type_def)
                throw GameValue_conversion_error("Invalid conversion to scalar");
#endif
        return data->get_as_bool();
    }
    return false;
}

GameValue::operator vector3() const
{
    if (!data)
    { return {}; }
    auto& array = data->get_as_array();
    if (array.count() == 3)
    {
        return vector3 {array[0], array[1], array[2]};
    }
    return vector3();
}

GameValue::operator vector2() const
{
    if (!data)
    { return {}; }
    auto& array = data->get_as_array();
    if (array.count() == 2)
    {
        return vector2 {array[0], array[1]};
    }
    return {};
}

GameValue::operator std::string() const
{
    if (data)
    {
        const auto type = data->get_vtable();
        if (type == game_data_code::type_def || type == game_data_string::type_def)
        {
            return static_cast<std::string>(data->get_as_string());
        }
        return static_cast<std::string>(data->to_string());
    }

    return {};
}

GameValue::operator r_string() const
{
    if (data)
    {
        const auto type = data->get_vtable();
        if (type == game_data_code::type_def || type == game_data_string::type_def)
        {
            return data->get_as_string();
        }
        return data->to_string();
    }

    return {};
}

auto_array<GameValue>& GameValue::to_array() const
{
    if (data)
    {
        if (data->get_vtable() != game_data_array::type_def)
        {
            throw GameValue_conversion_error(
                    "Invalid conversion to array"
            );
        }
        return data->get_as_array();
    }
    static auto_array<GameValue> dummy;//else we would return a temporary.
    dummy.clear(); //In case user modified it before
    return dummy;
}

GameValue& GameValue::operator[](size_t i_) const
{
    if (data)
    {
        if (data->get_vtable() != game_data_array::type_def)
        {
            throw GameValue_conversion_error(
                    "Invalid array access"
            );
        }
        auto& array = data->get_as_array();
        if (array.count() > i_)
        {
            return array[i_];
        }
    }
    static GameValue dummy;//else we would return a temporary.
    dummy.clear(); //In case user modified it before
    return dummy;
}

std::optional<GameValue> GameValue::get(size_t i_) const
{
    if (data)
    {
        if (data->get_vtable() != game_data_array::type_def)
        { return *this; }
        auto& array = data->get_as_array();
        if (array.count() > i_)
        {
            return array[i_];
        }
    }
    return {};
}

uintptr_t GameValue::type() const
{
    if (data)
    {
        return *reinterpret_cast<uintptr_t*>(data.get());
    } //#TODO use GetType virtual func instead
    return 0x0;
}

game_data_type GameValue::type_enum() const
{//#TODO make a static sorted table in the same order as enum. turns this search into a binary search
    if (!data)
    { return game_data_type::NOTHING; }
    const auto _type = data->get_vtable();
    if (_type == game_data_object::type_def)
    {
        return game_data_type::OBJECT;
    }
    if (_type == game_data_number::type_def)
    {
        return game_data_type::SCALAR;
    }
    if (_type == game_data_string::type_def)
    {
        return game_data_type::STRING;
    }
    if (_type == game_data_array::type_def)
    {
        return game_data_type::ARRAY;
    }
    if (_type == game_data_bool::type_def)
    {
        return game_data_type::BOOL;
    }
    if (_type == game_data_group::type_def)
    {
        return game_data_type::GROUP;
    }
    if (_type == game_data_config::type_def)
    {
        return game_data_type::CONFIG;
    }
    if (_type == game_data_control::type_def)
    {
        return game_data_type::CONTROL;
    }
    if (_type == game_data_display::type_def)
    {
        return game_data_type::DISPLAY;
    }
    if (_type == game_data_location::type_def)
    {
        return game_data_type::LOCATION;
    }
    if (_type == game_data_script::type_def)
    {
        return game_data_type::SCRIPT;
    }
    if (_type == game_data_side::type_def)
    {
        return game_data_type::SIDE;
    }
    if (_type == game_data_rv_text::type_def)
    {
        return game_data_type::TEXT;
    }
    if (_type == game_data_namespace::type_def)
    {
        return game_data_type::NAMESPACE;
    }
    if (_type == game_data_code::type_def)
    {
        return game_data_type::CODE;
    }
    if (_type == game_data_nothing::type_def)
    {
        return game_data_type::NOTHING;
    }
    return game_data_type::ANY;
}

size_t GameValue::size() const
{
    if (!data)
    { return 0; }
    if (data->get_vtable() != game_data_array::type_def)
    { return 0; }
    return data->get_as_array().count();
}

bool GameValue::is_nil() const
{
    if (!data)
    { return true; }
    return data->is_nil();
}

bool GameValue::is_null() const
{
    if (!data)
    { return true; }
    if (data->is_nil())
    { return true; }
    auto type = type_enum();
    switch (type)
    {

        case game_data_type::SCALAR:      //[[fallthrough]] //#TODO fix when MSVC gets their stuff together
        case game_data_type::BOOL:        //[[fallthrough]]
        case game_data_type::ARRAY:       //[[fallthrough]]
        case game_data_type::STRING:      //[[fallthrough]]
        case game_data_type::NOTHING:     //[[fallthrough]]
        case game_data_type::ANY:         //[[fallthrough]]
        case game_data_type::NAMESPACE:   //[[fallthrough]]
        case game_data_type::NaN:         //[[fallthrough]]
        case game_data_type::CODE:        //[[fallthrough]]
        case game_data_type::SIDE:        //[[fallthrough]]
        case game_data_type::TEXT:        //[[fallthrough]]
        case game_data_type::TARGET:      //[[fallthrough]]
        case game_data_type::NetObject:   //[[fallthrough]]
        case game_data_type::SUBGROUP:    //[[fallthrough]]
        case game_data_type::DIARY_RECORD:return false;
        case game_data_type::OBJECT:      //[[fallthrough]] //SL
        case game_data_type::GROUP:       //[[fallthrough]] //LL
        case game_data_type::SCRIPT:      //[[fallthrough]] //LL
        case game_data_type::DISPLAY:     //[[fallthrough]] //LL
        case game_data_type::CONTROL:     //[[fallthrough]] //LL
        case game_data_type::TEAM_MEMBER: //[[fallthrough]] //SLP
        case game_data_type::TASK:        //[[fallthrough]] //LL
        case game_data_type::LOCATION://LL
        {
            const uintptr_t datax = reinterpret_cast<uintptr_t>(data.get());
            const uintptr_t data_1 = datax + sizeof(uintptr_t) * 3;
            const uintptr_t data_2 = *reinterpret_cast<uintptr_t*>(data_1);
            if (data_2)
            {
                const uintptr_t data_3 = data_2 + sizeof(uintptr_t);
                const uintptr_t val = *reinterpret_cast<uintptr_t*>(data_3);
                return !val;
            }
            return true;
        }
        case game_data_type::CONFIG:
        {
            return !reinterpret_cast<game_data_config*>(data.get())->path.is_empty();//#TODO test
        }


        default:;
    }


    return false; //Dunno that Type. Users fault.
}

bool GameValue::operator==(const GameValue& other) const
{
    if (!data || !other.data)
    { return false; }
    if (data->type() != other.data->type())
    { return false; }
    return data->equals(other.data);

}

bool GameValue::operator!=(const GameValue& other) const
{
    if (!data || !other.data)
    { return true; }
    if (data->type() != other.data->type())
    { return true; }
    return !data->equals(other.data);
}

size_t GameValue::hash() const
{
    if (!data)
    { return 0; }

    switch (type_enum())
    {
        case game_data_type::SCALAR: return reinterpret_cast<game_data_number*>(data.get())->hash();
        case game_data_type::BOOL: return reinterpret_cast<game_data_bool*>(data.get())->hash();
        case game_data_type::ARRAY: return reinterpret_cast<game_data_array*>(data.get())->hash();
        case game_data_type::STRING: return reinterpret_cast<game_data_string*>(data.get())->hash();
        case game_data_type::NOTHING: return reinterpret_cast<game_data*>(data.get())->to_string().hash();
        case game_data_type::NAMESPACE: return reinterpret_cast<game_data_namespace*>(data.get())->hash();
        case game_data_type::NaN: return reinterpret_cast<game_data*>(data.get())->to_string().hash();
        case game_data_type::CODE: return reinterpret_cast<game_data_code*>(data.get())->hash();
        case game_data_type::OBJECT: return reinterpret_cast<game_data_object*>(data.get())->hash();
        case game_data_type::SIDE: return reinterpret_cast<game_data_side*>(data.get())->hash();
        case game_data_type::GROUP: return reinterpret_cast<game_data_group*>(data.get())->hash();
        case game_data_type::TEXT: return reinterpret_cast<game_data_rv_text*>(data.get())->hash();
        case game_data_type::SCRIPT: return reinterpret_cast<game_data_script*>(data.get())->hash();
        case game_data_type::TARGET: return reinterpret_cast<game_data*>(data.get())->to_string().hash();
        case game_data_type::CONFIG: return reinterpret_cast<game_data_config*>(data.get())->hash();
        case game_data_type::DISPLAY: return reinterpret_cast<game_data_display*>(data.get())->hash();
        case game_data_type::CONTROL: return reinterpret_cast<game_data_control*>(data.get())->hash();
#if defined(_DEBUG) && defined(_MSC_VER)
            //If you encounter any of these give Dedmen a repro.
        case game_data_type::ANY: __debugbreak();
            break;//ANY should never be seen as a value.
        case game_data_type::NetObject: __debugbreak();
            break;
        case game_data_type::SUBGROUP: __debugbreak();
            break;
#else
            case game_data_type::ANY: return 0;
            case game_data_type::NetObject: return 0;
            case game_data_type::SUBGROUP: return 0;
#endif
        case game_data_type::TEAM_MEMBER: return reinterpret_cast<game_data_team_member*>(data.get())->hash();
        case game_data_type::TASK:
            return reinterpret_cast<game_data*>(data.get())->to_string()
                                                           .hash(); //"Task %s (id %d)" or "No Task"
        case game_data_type::DIARY_RECORD:
            return reinterpret_cast<game_data*>(data.get())->to_string()
                                                           .hash(); //"No diary record" or... The text of that record? Text might be long and make this hash heavy
        case game_data_type::LOCATION: return reinterpret_cast<game_data_location*>(data.get())->hash();
        case game_data_type::end: return 0;
    }

    return _private::pairhash<uintptr_t, uintptr_t>(
            data->get_vtable(), reinterpret_cast<uintptr_t>(data.get()));
}

void* GameValue::operator new(const std::size_t sz_)
{
    return rv_allocator<GameValue>::create_array(sz_);
}

void GameValue::operator delete(void* ptr_, std::size_t)
{
    rv_allocator<GameValue>::deallocate(static_cast<GameValue*>(ptr_));
}
