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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_H

#include "serialization.h"
#include "game_data.h"
#include "game_type.h"
#include "../vector.h"
#include <vector>

namespace mafia::game_types
{
    class GameValue: public mafia::game_types::SerializeClass
    {
        // friend class mafia::invoker;
        // friend void mafia::set_game_value_vtable(uintptr_t);

    protected:
        static uintptr_t __vptr_def;  //Users should not be able to access this
    public:
        GameValue() noexcept
        {
            set_vtable(__vptr_def);
        }

        ~GameValue() noexcept {}

        void copy(const GameValue& copy_) noexcept
        {
            set_vtable(__vptr_def);  //Whatever vtable copy_ has.. if it's different then it's wrong
            data = copy_.data;
        }

        GameValue(const GameValue& copy_)
        {  //I don't see any use for this.
            copy(copy_);
        }

#pragma optimize("ts", on)

        GameValue(GameValue&& move_) noexcept
        {
            set_vtable(__vptr_def);  //Whatever vtable move_ has.. if it's different then it's wrong
            data = nullptr;
            data.swap(move_.data);
        }

#pragma optimize("", on)

        //Conversions
        GameValue(GameData* val_) noexcept
        {
            set_vtable(__vptr_def);
            data = val_;
        }

        GameValue(float val_);
        GameValue(int val_);
        GameValue(size_t val_);
        GameValue(bool val_);
        GameValue(const std::string& val_);
        GameValue(const mafia::game_types::String& val_);
        GameValue(std::string_view val_);

        GameValue(const char* str_): GameValue(std::string_view(str_)) {}

        GameValue(const std::vector<GameValue>& list_);
        GameValue(const std::initializer_list<GameValue>& list_);
        GameValue(auto_array<GameValue>&& array_);

        template<class Type>
        GameValue(const auto_array<Type>& array_) :
                GameValue(auto_array<game_value>(array_.begin(), array_.end()))
        {
            static_assert(std::is_convertible<Type, game_value>::value);
        }

        template<class Type>
        GameValue(const std::vector<Type>& array_) :
                GameValue(auto_array<game_value>(array_.begin(), array_.end()))
        {
            static_assert(std::is_convertible<Type, game_value>::value);
        }

        GameValue(const vector3& vec_);
        GameValue(const vector2& vec_);
        //GameValue(const internal_object& internal_);

        GameValue& operator=(const GameValue& copy_);
        GameValue& operator=(GameValue&& move_) noexcept;
        GameValue& operator=(float val_);
        GameValue& operator=(bool val_);
        GameValue& operator=(const std::string& val_);
        GameValue& operator=(const mafia::game_types::String& val_);
        GameValue& operator=(std::string_view val_);

        GameValue& operator=(const char* str_)
        {
            return this->operator=(std::string_view(str_));
        }

        GameValue& operator=(const std::vector<GameValue>& list_);
        GameValue& operator=(const vector3& vec_);
        GameValue& operator=(const vector2& vec_);
        //GameValue& operator=(const internal_object& internal_);

        operator int() const;
        operator float() const;
        operator bool() const;
        operator std::string() const;
        operator mafia::game_types::String() const;
        operator vector3() const;
        operator vector2() const;

        /**
        * @brief tries to convert the game_value to an array if possible
        * @throws game_value_conversion_error {if game_value is not an array}
        */
        auto_array<game_value>& to_array() const;

        /**
        * @brief tries to convert the game_value to an array if possible and return the element at given index.
        * @throw game_value_conversion_error {if game_value is not an array}
        */
        game_value& operator[](size_t i_) const;

        /**
        * @brief tries to convert the game_value to an array if possible and return the element at given index.
        * @description If value is not an array and index==0 it returns the value.
        * If the index is out of bounds it returns empty optional.
        */
        std::optional<game_value> get(size_t i_) const;

        uintptr_t
        type() const;  //#TODO should this be renamed to type_vtable? and make the enum variant the default? We still want to use vtable internally cuz speed
        /// doesn't handle all types. Will return game_data_type::ANY if not handled
        mafia::game_types::GameType type_enum() const;

        size_t size() const;
        //#TODO implement is_null. GameDataObject's objectLink == nullptr. Same for GameDataGroup and others.
        /// @brief Returns true for uninitialized game_value's and Nil values returned from Arma
        bool is_nil() const;
        /// @brief Trying to replicate SQF isNull as good as possible. It combines isNil and isNull.
        bool is_null() const;

        bool operator==(const game_value& other) const;
        bool operator!=(const game_value& other) const;

        size_t hash() const;

        //set's this game_value to null
        void clear() { data = nullptr; }

        SerializationReturn serialize(ParamArchive& ar) override;

        mafia::game_types::Ref<GameData> data;

        template<class T>
        mafia::game_types::Ref<T> get_as()
        {
            static_assert(
                    std::is_convertible_v<T, game_data>, "GameValue::get_as() can only convert to GameData types"
            );
            return static_cast<ref<T>>(data);
        }

        template<class T>
        const mafia::game_types::Ref<T> get_as() const
        {
            static_assert(
                    std::is_convertible_v<T, game_data>, "GameValue::get_as() can only convert to GameData types"
            );
            return static_cast<const ref<T>>(data);
        }


        static void operator delete(void* ptr_, std::size_t sz_);
#ifndef __linux__
    protected:
#endif

        uintptr_t get_vtable() const noexcept
        {
            return *reinterpret_cast<const uintptr_t*>(this);
        }

#pragma optimize("ts", on)

        void set_vtable(uintptr_t vt) noexcept
        {
            *reinterpret_cast<uintptr_t*>(this) = vt;
        }

#pragma optimize("", on)
    };

    class GameValueConversionError: public std::runtime_error
    {
    public:
        explicit GameValueConversionError(const std::string& _Message)
                : runtime_error(_Message) {}

        explicit GameValueConversionError(const char* _Message)
                : runtime_error(_Message) {}
    };

    class GameValueStatic: public GameValue
    {
    public:
        GameValueStatic(): game_value() {}

        ~GameValueStatic();

        GameValueStatic(const GameValue& copy): GameValue(copy) {}

        GameValueStatic(GameValue&& move): GameValue(move) {}

        GameValueStatic& operator=(const GameValue& copy)
        {
            data = copy.data;
            return *this;
        }
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_H
