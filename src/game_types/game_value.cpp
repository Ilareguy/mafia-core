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
#include "game_value_impl.h"
#include "game_data/all.h"
#include "param_archive.h"
#include "sqf_script_type.h"
#include "../pair_hash.h"
#include "../mafia.h"

using namespace mafia::game_types;
using namespace std::literals::string_view_literals;

uintptr_t GameValue::__vptr_def {0};

GameValue::GameValue() noexcept
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
}

GameValue::~GameValue() noexcept
{
    delete impl;
}

GameValue::GameValue(GameData* val_) noexcept
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = val_;
}

GameValue::GameValue(float val_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Number(val_);
}

GameValue::operator mafia::game_types::String() const
{
    if (impl->data)
    {
        const auto type = impl->data->get_vtable();
        if (type == game_data::Code::type_def || type == game_data::String::type_def)
        {
            return impl->data->get_as_string();
        }
        return impl->data->to_string();
    }

    return {};
}

void GameValue::copy(const GameValue& copy_) noexcept
{
    set_vtable(__vptr_def);  //Whatever vtable copy_ has.. if it's different then it's wrong
    impl->data = copy_.impl->data;
}

GameValue::GameValue(const GameValue& copy_) noexcept
{
    impl = new GameValueImpl;
    copy(copy_);
}

#pragma optimize("ts", on)

GameValue::GameValue(GameValue&& move_) noexcept
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);  //Whatever vtable move_ has.. if it's different then it's wrong
    impl->data = nullptr;
    impl->data.swap(move_.impl->data);
}

#pragma optimize("", on)

GameValue::GameValue(int val_): GameValue(static_cast<float>(val_)) {}

GameValue::GameValue(size_t val_): GameValue(static_cast<float>(val_)) {}

GameValue::GameValue(bool val_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Bool(val_);
}

GameValue::GameValue(const std::string& val_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::String(val_);
}

GameValue::GameValue(const String& val_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::String(val_);
}

GameValue::GameValue(std::string_view val_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::String(val_);
}

GameValue::GameValue(const std::vector<GameValue>& list_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Array(list_);
}

GameValue::GameValue(const std::initializer_list<GameValue>& list_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Array(list_);
}

GameValue::GameValue(mafia::containers::AutoArray<GameValue>&& array_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Array(std::move(array_));
}

GameValue::GameValue(const vector3& vec_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Array({vec_.x, vec_.y, vec_.z});
}

GameValue::GameValue(const vector2& vec_)
{
    impl = new GameValueImpl;
    set_vtable(__vptr_def);
    impl->data = new game_data::Array({vec_.x, vec_.y});
}

/*GameValue::GameValue(const internal_object& internal_)
{
    set_vtable(__vptr_def); //object class has bugged vtable :u
    data = internal_.data;
}*/

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
    impl->data = move_.impl->data;
    move_.impl->data = nullptr;
    return *this;
}

GameValue& GameValue::operator=(const float val_)
{
    impl->data = new game_data::Number(val_);
    return *this;
}

GameValue& GameValue::operator=(bool val_)
{
    impl->data = new game_data::Bool(val_);
    return *this;
}

GameValue& GameValue::operator=(const std::string& val_)
{
    impl->data = new game_data::String(val_);
    return *this;
}

GameValue& GameValue::operator=(const String& val_)
{
    impl->data = new game_data::String(val_);
    return *this;
}

GameValue& GameValue::operator=(std::string_view val_)
{
    impl->data = new game_data::String(val_);
    return *this;
}

GameValue& GameValue::operator=(const char* str_)
{
    return this->operator=(std::string_view(str_));
}

GameValue& GameValue::operator=(const std::vector<GameValue>& list_)
{
    impl->data = new game_data::Array(list_);
    return *this;
}

GameValue& GameValue::operator=(const vector3& vec_)
{
    impl->data = new game_data::Array({vec_.x, vec_.y, vec_.z});
    return *this;
}

GameValue& GameValue::operator=(const vector2& vec_)
{
    impl->data = new game_data::Array({vec_.x, vec_.y});
    return *this;
}

/*GameValue& GameValue::operator=(const internal_object& internal_)
{
    data = internal_.data;
    set_vtable(__vptr_def);
    return *this;
}*/

GameValue::operator int() const
{
    if (impl->data)
    {
#ifdef MAFIA_SAFE_CONVERSIONS
        const auto type = impl->data->get_vtable();
            if (impl->data->get_vtable() == game_data_number::type_def)
                throw GameValue_conversion_error("Invalid conversion to scalar");
#endif
        return static_cast<int>(impl->data->get_as_number());
    }

    return 0;
}

GameValue::operator float() const
{
    if (impl->data)
    {
#ifdef MAFIA_SAFE_CONVERSIONS
        const auto type = impl->data->get_vtable();
            if (impl->data->get_vtable() == game_data_number::type_def)
                throw GameValue_conversion_error("Invalid conversion to scalar");
#endif
        return impl->data->get_as_number();
    }
    return 0.f;
}

GameValue::operator bool() const
{
    if (impl->data)
    {
#ifdef MAFIA_SAFE_CONVERSIONS
        const auto type = impl->data->get_vtable();
            if (impl->data->get_vtable() == game_data_bool::type_def)
                throw GameValue_conversion_error("Invalid conversion to scalar");
#endif
        return impl->data->get_as_bool();
    }
    return false;
}

GameValue::operator mafia::vector3() const
{
    if (!impl->data)
    { return {}; }
    auto& array = impl->data->get_as_array();
    if (array.count() == 3)
    {
        return vector3 {array[0], array[1], array[2]};
    }
    return vector3();
}

GameValue::operator mafia::vector2() const
{
    if (!impl->data)
    { return {}; }
    auto& array = impl->data->get_as_array();
    if (array.count() == 2)
    {
        return vector2 {array[0], array[1]};
    }
    return {};
}

GameValue::operator std::string() const
{
    if (impl->data)
    {
        const auto type = impl->data->get_vtable();
        if (type == game_data::Code::type_def || type == game_data::String::type_def)
        {
            return static_cast<std::string>(impl->data->get_as_string());
        }
        return static_cast<std::string>(impl->data->to_string());
    }

    return {};
}

mafia::containers::AutoArray<GameValue>& GameValue::to_array() const
{
    if (impl->data)
    {
        if (impl->data->get_vtable() != game_data::Array::type_def)
        {
            throw GameValueConversionError(
                    "Invalid conversion to array"
            );
        }
        return impl->data->get_as_array();
    }
    static mafia::containers::AutoArray<GameValue> dummy;//else we would return a temporary.
    dummy.clear(); //In case user modified it before
    return dummy;
}

GameValue& GameValue::operator[](size_t i_) const
{
    if (impl->data)
    {
        if (impl->data->get_vtable() != game_data::Array::type_def)
        {
            throw GameValueConversionError(
                    "Invalid array access"
            );
        }
        auto& array = impl->data->get_as_array();
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
    if (impl->data)
    {
        if (impl->data->get_vtable() != game_data::Array::type_def)
        { return *this; }
        auto& array = impl->data->get_as_array();
        if (array.count() > i_)
        {
            return array[i_];
        }
    }
    return {};
}

uintptr_t GameValue::type() const
{
    if (impl->data)
    {
        return *reinterpret_cast<uintptr_t*>(impl->data.get());
    } //#TODO use GetType virtual func instead
    return 0x0;
}

GameDataType GameValue::type_enum() const
{
    if (!impl->data)
    { return GameDataType::NOTHING; }
    const auto _type = impl->data->get_vtable();
    if (_type == game_data::Object::type_def)
    {
        return GameDataType::OBJECT;
    }
    if (_type == game_data::Number::type_def)
    {
        return GameDataType::SCALAR;
    }
    if (_type == game_data::String::type_def)
    {
        return GameDataType::STRING;
    }
    if (_type == game_data::Array::type_def)
    {
        return GameDataType::ARRAY;
    }
    if (_type == game_data::Bool::type_def)
    {
        return GameDataType::BOOL;
    }
    if (_type == game_data::Group::type_def)
    {
        return GameDataType::GROUP;
    }
    if (_type == game_data::Config::type_def)
    {
        return GameDataType::CONFIG;
    }
    if (_type == game_data::Control::type_def)
    {
        return GameDataType::CONTROL;
    }
    if (_type == game_data::Display::type_def)
    {
        return GameDataType::DISPLAY;
    }
    if (_type == game_data::Location::type_def)
    {
        return GameDataType::LOCATION;
    }
    if (_type == game_data::Script::type_def)
    {
        return GameDataType::SCRIPT;
    }
    if (_type == game_data::Side::type_def)
    {
        return GameDataType::SIDE;
    }
    if (_type == game_data::RVText::type_def)
    {
        return GameDataType::TEXT;
    }
    if (_type == game_data::Namespace::type_def)
    {
        return GameDataType::NAMESPACE;
    }
    if (_type == game_data::Code::type_def)
    {
        return GameDataType::CODE;
    }
    if (_type == game_data::Nothing::type_def)
    {
        return GameDataType::NOTHING;
    }
    return GameDataType::ANY;
}

size_t GameValue::size() const
{
    if (!impl->data)
    { return 0; }
    if (impl->data->get_vtable() != game_data::Array::type_def)
    { return 0; }
    return impl->data->get_as_array().count();
}

bool GameValue::is_nil() const
{
    if (!impl->data)
    { return true; }
    return impl->data->is_nil();
}

bool GameValue::is_null() const
{
    if (!impl->data)
    { return true; }
    if (impl->data->is_nil())
    { return true; }
    auto type = type_enum();
    switch (type)
    {

        case GameDataType::SCALAR:      //[[fallthrough]] //#TODO fix when MSVC gets their stuff together
        case GameDataType::BOOL:        //[[fallthrough]]
        case GameDataType::ARRAY:       //[[fallthrough]]
        case GameDataType::STRING:      //[[fallthrough]]
        case GameDataType::NOTHING:     //[[fallthrough]]
        case GameDataType::ANY:         //[[fallthrough]]
        case GameDataType::NAMESPACE:   //[[fallthrough]]
        case GameDataType::NaN:         //[[fallthrough]]
        case GameDataType::CODE:        //[[fallthrough]]
        case GameDataType::SIDE:        //[[fallthrough]]
        case GameDataType::TEXT:        //[[fallthrough]]
        case GameDataType::TARGET:      //[[fallthrough]]
        case GameDataType::NetObject:   //[[fallthrough]]
        case GameDataType::SUBGROUP:    //[[fallthrough]]
        case GameDataType::DIARY_RECORD:return false;
        case GameDataType::OBJECT:      //[[fallthrough]] //SL
        case GameDataType::GROUP:       //[[fallthrough]] //LL
        case GameDataType::SCRIPT:      //[[fallthrough]] //LL
        case GameDataType::DISPLAY:     //[[fallthrough]] //LL
        case GameDataType::CONTROL:     //[[fallthrough]] //LL
        case GameDataType::TEAM_MEMBER: //[[fallthrough]] //SLP
        case GameDataType::TASK:        //[[fallthrough]] //LL
        case GameDataType::LOCATION://LL
        {
            const uintptr_t datax = reinterpret_cast<uintptr_t>(impl->data.get());
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
        case GameDataType::CONFIG:
        {
            return !reinterpret_cast<game_data::Config*>(impl->data.get())->path.is_empty();
        }


        default:;
    }


    return false; //Dunno that Type. Users fault.
}

bool GameValue::operator==(const GameValue& other) const
{
    if (!impl->data || !other.impl->data)
    { return false; }
    if (impl->data->type() != other.impl->data->type())
    { return false; }
    return impl->data->equals(other.impl->data);

}

bool GameValue::operator!=(const GameValue& other) const
{
    if (!impl->data || !other.impl->data)
    { return true; }
    if (impl->data->type() != other.impl->data->type())
    { return true; }
    return !impl->data->equals(other.impl->data);
}

size_t GameValue::hash() const
{
    if (!impl->data)
    { return 0; }

    switch (type_enum())
    {
        case GameDataType::SCALAR: return reinterpret_cast<game_data::Number*>(impl->data.get())->hash();
        case GameDataType::BOOL: return reinterpret_cast<game_data::Bool*>(impl->data.get())->hash();
        case GameDataType::ARRAY: return reinterpret_cast<game_data::Array*>(impl->data.get())->hash();
        case GameDataType::STRING: return reinterpret_cast<game_data::String*>(impl->data.get())->hash();
        case GameDataType::NOTHING: return reinterpret_cast<GameData*>(impl->data.get())->to_string().hash();
        case GameDataType::NAMESPACE: return reinterpret_cast<game_data::Namespace*>(impl->data.get())->hash();
        case GameDataType::NaN: return reinterpret_cast<GameData*>(impl->data.get())->to_string().hash();
        case GameDataType::CODE: return reinterpret_cast<game_data::Code*>(impl->data.get())->hash();
        case GameDataType::OBJECT: return reinterpret_cast<game_data::Object*>(impl->data.get())->hash();
        case GameDataType::SIDE: return reinterpret_cast<game_data::Side*>(impl->data.get())->hash();
        case GameDataType::GROUP: return reinterpret_cast<game_data::Group*>(impl->data.get())->hash();
        case GameDataType::TEXT: return reinterpret_cast<game_data::RVText*>(impl->data.get())->hash();
        case GameDataType::SCRIPT: return reinterpret_cast<game_data::Script*>(impl->data.get())->hash();
        case GameDataType::TARGET: return reinterpret_cast<GameData*>(impl->data.get())->to_string().hash();
        case GameDataType::CONFIG: return reinterpret_cast<game_data::Config*>(impl->data.get())->hash();
        case GameDataType::DISPLAY: return reinterpret_cast<game_data::Display*>(impl->data.get())->hash();
        case GameDataType::CONTROL: return reinterpret_cast<game_data::Control*>(impl->data.get())->hash();
        case GameDataType::ANY: return 0;
        case GameDataType::NetObject: return 0;
        case GameDataType::SUBGROUP: return 0;
        case GameDataType::TEAM_MEMBER: return reinterpret_cast<game_data::TeamMember*>(impl->data.get())->hash();
        case GameDataType::TASK:
            return reinterpret_cast<GameData*>(impl->data.get())->to_string()
                                                          .hash(); //"Task %s (id %d)" or "No Task"
        case GameDataType::DIARY_RECORD:
            return reinterpret_cast<GameData*>(impl->data.get())->to_string()
                                                          .hash(); //"No diary record" or... The text of that record? Text might be long and make this hash heavy
        case GameDataType::LOCATION: return reinterpret_cast<game_data::Location*>(impl->data.get())->hash();
        case GameDataType::end: return 0;
    }

    return mafia::pair_hash<uintptr_t, uintptr_t>(
            impl->data->get_vtable(), reinterpret_cast<uintptr_t>(impl->data.get()));
}

void GameValue::clear() { impl->data = nullptr; }

/*void* GameValue::operator new(const std::size_t sz_)
{
    return RVAllocator<GameValue>::create_array(sz_);
}*/

void GameValue::operator delete(void* ptr_, std::size_t)
{
    RVAllocator<GameValue>::deallocate(static_cast<GameValue*>(ptr_));
}

uintptr_t GameValue::get_vtable() const noexcept
{
    return *reinterpret_cast<const uintptr_t*>(this);
}

#pragma optimize("ts", on)

void GameValue::set_vtable(uintptr_t vt) noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = vt;
}

#pragma optimize("", on)

SerializationReturn GameValue::serialize(ParamArchive& ar)
{
    if (!impl->data)
    { impl->data = RVAllocator<game_data::Nothing>::create_single(); }
    ar.serialize(String("data"sv), impl->data, 1);

    if (impl->data && impl->data->get_vtable() == game_data::Nothing::type_def)
    { impl->data = nullptr; }
    return SerializationReturn::no_error;
}

GameValue::GameValue(const char* str_): GameValue(std::string_view(str_)) {}

GameValueStatic::GameValueStatic(): GameValue() {}

GameValueStatic::~GameValueStatic()
{
    if (mafia::is_exiting())
    { impl->data._ref = nullptr; }
}

/*GameValueStatic::GameValueStatic(const GameValue& copy): GameValue(copy) {}

GameValueStatic::GameValueStatic(GameValue&& move): GameValue(move) {}*/

GameValueStatic& GameValueStatic::operator=(const GameValue& copy)
{
    impl->data = copy.impl->data;
    return *this;
}
