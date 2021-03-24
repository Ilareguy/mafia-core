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
#include "game_data_type.h"
#include "../ref.h"
#include "../shared.h"
#include "../vector.h"
#include "../containers/auto_array.h"
#include <vector>
#include <optional>

namespace mafia
{
    class Invoker;
}

namespace mafia::game_types
{
    class GameData;
    class GameValueImpl;

    class GameValue: public mafia::game_types::SerializeClass
    {
        friend void set_game_value_vtable(uintptr_t vtable);
        friend class mafia::Invoker;

    protected:
        static uintptr_t __vptr_def;  //Users should not be able to access this

    public:
        GameValue() noexcept;
        ~GameValue() noexcept;
        void copy(const GameValue& copy_) noexcept;
        GameValue(const GameValue& copy_) noexcept;
        GameValue(GameValue&& move_) noexcept;

        //Conversions
        GameValue(GameData* val_) noexcept;
        GameValue(float val_);
        GameValue(int val_);
        GameValue(size_t val_);
        GameValue(bool val_);
        GameValue(const std::string& val_);
        GameValue(const mafia::game_types::String& val_);
        GameValue(std::string_view val_);
        GameValue(const char* str_);
        GameValue(const std::vector<GameValue>& list_);
        GameValue(const std::initializer_list<GameValue>& list_);
        GameValue(mafia::containers::AutoArray<GameValue>&& array_);
        GameValue(const vector3& vec_);
        GameValue(const vector2& vec_);

        template<class Type>
        GameValue(const mafia::containers::AutoArray<Type>& array_) :
                GameValue(mafia::containers::AutoArray<GameValue>(array_.begin(), array_.end()))
        {
            static_assert(std::is_convertible<Type, GameValue>::value);
        }

        template<class Type>
        GameValue(const std::vector<Type>& array_) :
                GameValue(mafia::containers::AutoArray<GameValue>(array_.begin(), array_.end()))
        {
            static_assert(std::is_convertible<Type, GameValue>::value);
        }

        //GameValue(const internal_object& internal_);
        GameValue& operator=(const GameValue& copy_);
        GameValue& operator=(GameValue&& move_) noexcept;
        GameValue& operator=(float val_);
        GameValue& operator=(bool val_);
        GameValue& operator=(const std::string& val_);
        GameValue& operator=(const mafia::game_types::String& val_);
        GameValue& operator=(std::string_view val_);
        GameValue& operator=(const char* str_);
        GameValue& operator=(const std::vector<GameValue>& list_);
        GameValue& operator=(const vector3& vec_);
        GameValue& operator=(const vector2& vec_);
        //GameValue& operator=(const internal_object& internal_);

        operator int() const;
        operator float() const;
        operator bool() const;
        operator std::string() const;
        operator mafia::game_types::String() const;
        operator mafia::vector3() const;
        operator mafia::vector2() const;

        /**
        * @brief tries to convert the game_value to an array if possible
        * @throws game_value_conversion_error {if game_value is not an array}
        */
        mafia::containers::AutoArray<GameValue>& to_array() const;

        /**
        * @brief tries to convert the game_value to an array if possible and return the element at given index.
        * @throw game_value_conversion_error {if game_value is not an array}
        */
        GameValue& operator[](size_t i_) const;

        /**
        * @brief tries to convert the game_value to an array if possible and return the element at given index.
        * @description If value is not an array and index==0 it returns the value.
        * If the index is out of bounds it returns empty optional.
        */
        std::optional<GameValue> get(size_t i_) const;

        uintptr_t type() const;
        /// doesn't handle all types. Will return game_data_type::ANY if not handled
        mafia::game_types::GameDataType type_enum() const;

        size_t size() const;
        //#TODO implement is_null. GameDataObject's objectLink == nullptr. Same for GameDataGroup and others.
        /// @brief Returns true for uninitialized game_value's and Nil values returned from Arma
        bool is_nil() const;
        /// @brief Trying to replicate SQF isNull as good as possible. It combines isNil and isNull.
        bool is_null() const;

        bool operator==(const GameValue& other) const;
        bool operator!=(const GameValue& other) const;

        size_t hash() const;

        //sets this GameValue to null
        void clear();

        SerializationReturn serialize(ParamArchive& ar) override;

        // `data` is defined in game_value_impl.h. Include that file if you need to access it.
        //mafia::Ref<GameData> data;
        GameValueImpl* impl;

        static void operator delete(void* ptr_, std::size_t sz_);
#ifndef __linux__
    protected:
#endif

        uintptr_t get_vtable() const noexcept;
        void set_vtable(uintptr_t vt) noexcept;
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
        GameValueStatic();
        ~GameValueStatic();
        //GameValueStatic(const GameValue& copy);
        //GameValueStatic(GameValue&& move);
        GameValueStatic& operator=(const GameValue& copy);
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_VALUE_H
