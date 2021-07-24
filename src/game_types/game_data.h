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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H

#include "debug_value.h"
#include "serialization.h"
#include "game_data_type.h"
#include "../ref_count.h"
#include "../ref.h"
#include "../shared.h"
#include "../vector.h"
#include "../containers/auto_array.h"
#include "../containers/auto_array.h"
#include <cstddef>
#include <vector>
#include <optional>

namespace mafia
{
    class Invoker;
}

namespace mafia::game_types
{
    class RVPoolAllocator;
    class SQFScriptType;
    class GameValue;

    class GameData: public mafia::RefCount,
                    public mafia::game_types::DebugValue
    {
        /**
         * Note: The order of members in this class matters because it is polymorphic and instances of this type (and
         * their sub-types) are created by Arma/the RV engine.
         */

        friend class GameValue;
        friend class mafia::Invoker;

    public:
        virtual const SQFScriptType& type() const;
        virtual ~GameData();

    protected:
        virtual bool get_as_bool() const;
        virtual float get_as_number() const;
        /// Only usable on String and Code! Use to_string instead!
        virtual const mafia::game_types::String& get_as_string() const;
        /// Why would you ever need this?
        virtual const mafia::containers::AutoArray<mafia::game_types::GameValue>& get_as_const_array() const;
        virtual mafia::containers::AutoArray<mafia::game_types::GameValue>& get_as_array();

    public:
        virtual mafia::game_types::GameData* copy() const;

    protected:
        virtual void set_readonly(bool); //No clue what this does...
        virtual bool get_readonly() const;
        virtual bool get_final() const;
        virtual void set_final(bool); ///Only on GameDataCode AFAIK
    public:
        virtual mafia::game_types::String to_string() const;
        virtual bool equals(const mafia::game_types::GameData*) const;
        virtual const char* type_as_string() const;
        virtual bool is_nil() const;

    private:
        virtual void placeholder() const;
        virtual bool can_serialize();
        int IaddRef() override;
        int Irelease() override;

    public:
        virtual mafia::game_types::SerializationReturn serialize(mafia::game_types::ParamArchive& ar);
        static mafia::game_types::GameData* createFromSerialized(mafia::game_types::ParamArchive& ar);

#ifdef __linux__
        public:
#endif
        uintptr_t get_vtable() const;
        uintptr_t get_secondary_vtable() const noexcept;
    };
}

namespace mafia
{
    class Invoker;
}

namespace mafia::game_types
{
    class GameData;
    class InternalObject;

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

        GameValue(const InternalObject& internal_);
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
        GameValue& operator=(const InternalObject& internal_);

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
        /// doesn't dll_handle all types. Will return game_data_type::ANY if not handled
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

        mafia::Ref<GameData> data;

        [[deprecated]] static void* operator new(std::size_t sz_);  //Should never be used
        static void operator delete(void* ptr_, std::size_t sz_);
#ifndef __linux__
    protected:
#endif

        uintptr_t get_vtable() const noexcept;
        void set_vtable(uintptr_t vt) noexcept;

    public:
        template<class T>
        mafia::Ref<T> get_as()
        {
            static_assert(
                    std::is_convertible_v<T, GameData>, "GameValue::get_as() can only convert to GameData types"
            );
            return static_cast<mafia::Ref<T>>(data);
        }

        template<class T>
        const mafia::Ref<T> get_as() const
        {
            static_assert(
                    std::is_convertible_v<T, GameData>, "GameValue::get_as() can only convert to GameData types"
            );
            return static_cast<const mafia::Ref<T>>(data);
        }
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

namespace std
{
    template<>
    struct hash<mafia::game_types::GameValue>
    {
        size_t operator()(const mafia::game_types::GameValue& x) const
        {
            return x.hash();
        }
    };

    //custom conversion from std::string& to const std::string& inside reference_wrapper
    template<>
    class reference_wrapper<const std::string>
            : public reference_wrapper<std::string> {
    public:
        reference_wrapper(string& _Val) noexcept
                : reference_wrapper<std::string>(_Val) {};
    };
}  // namespace std

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_H
