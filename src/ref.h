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

#ifndef DEF_MAFIA_CORE_REF_H
#define DEF_MAFIA_CORE_REF_H

#include "ref_count.h"
#include <type_traits>

namespace mafia
{
    namespace game_types
    {
        class GameValueStatic;
    }

    template<class Type>
    class Ref
    {
        friend class mafia::game_types::GameValueStatic;  //Overrides _ref to nullptr in destructor when Arma is exiting
        static_assert(std::is_base_of<mafia::RefCountBase, Type>::value, "Type must inherit mafia::RefCountBase");
        Type* _ref {nullptr};

    public:
        using baseType = Type;

        constexpr Ref() noexcept = default;

        ~Ref() { free(); }

        //Construct from Pointer
        constexpr Ref(
                Type
                * other_
        ) noexcept
        {
            if (other_)
            { other_->add_ref(); }
            _ref = other_;
        }

        //Copy from pointer
        Ref& operator=(Type* source_) noexcept
        {
            Type* old = _ref;
            if (source_)
            { source_->add_ref(); }
            _ref = source_;
            if (old)
            { old->release(); }  //decrement reference and delete object if refcount == 0
            return *this;
        }

        //Construct from reference
        constexpr Ref(const Ref& source_ref_) noexcept
        {
            Type* source = source_ref_._ref;
            if (source)
            { source->add_ref(); }
            _ref = source;
        }

        //Copy from reference.
        Ref& operator=(const Ref& other_)
        {
            Type* source = other_._ref;
            Type* old = _ref;
            if (source)
            { source->add_ref(); }
            _ref = source;
            if (old)
            { old->release(); }  //decrement reference and delete object if refcount == 0
            return *this;
        }

        //Construct from reference and convert
        template<typename T>
        constexpr Ref(const Ref<T>& source_ref_) noexcept
        {
            static_assert(
                    std::is_constructible_v < Type * , T * > || std::is_base_of_v < T, Type > ,
                    "Cannot convert mafia::Ref to incompatible type"
            );
            T* source = source_ref_.get();
            if (source)
            { source->add_ref(); }
            _ref = static_cast<Type*>(source);
        }

        //Copy from reference.
        template<class T>
        Ref& operator=(const Ref<T>& other_)
        {
            static_assert(
                    std::is_constructible_v < Type * , T * > || std::is_base_of_v < T, Type > ,
                    "Cannot convert mafia::Ref to incompatible type"
            );
            T* source = other_.get();
            Type* old = _ref;
            if (source)
            { source->add_ref(); }
            _ref = source;
            if (old)
            { old->release(); }  //decrement reference and delete object if refcount == 0
            return *this;
        }

        void swap(Ref& other_) noexcept
        {
            auto temp = other_._ref;
            other_._ref = _ref;
            _ref = temp;
        }

        constexpr bool is_null() const noexcept { return _ref == nullptr; }

        void free() noexcept
        {
            if (!_ref)
            { return; }
            _ref->release();
            _ref = nullptr;
        }

        ///This returns a pointer to the underlying object. Use with caution!
        constexpr Type* get() const noexcept { return _ref; }

        constexpr Type* operator->() const noexcept { return _ref; }

        operator Type*() const noexcept { return _ref; }

        bool operator!=(const Ref <Type>& other_) const noexcept { return _ref != other_._ref; }

        size_t ref_count() const noexcept { return _ref ? _ref->ref_count() : 0; }
    };
}

#endif // DEF_MAFIA_CORE_REF_H
