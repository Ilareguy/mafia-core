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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_DEBUG_VALUE_H
#define DEF_MAFIA_CORE_GAME_TYPES_DEBUG_VALUE_H

#include "ref.h"

namespace mafia::game_types
{
    namespace _private
    {
        //specialization for I_debug_value. You should not be using this.
        template<class Type>
        class i_ref
        {
            Type* _ref {nullptr};

        public:
            using baseType = Type;
            constexpr i_ref() noexcept = default;

            ~i_ref() { free(); }

            //Construct from Pointer
            constexpr i_ref(Type* other_) noexcept
            {
                if (other_)
                { other_->IaddRef(); }
                _ref = other_;
            }

            //Copy from pointer
            i_ref& operator=(Type* source_) noexcept
            {
                Type* old = _ref;
                if (source_)
                { source_->IaddRef(); }
                _ref = source_;
                if (old)
                { old->release(); }  //decrement reference and delete object if refcount == 0
                return *this;
            }

            //Construct from reference
            constexpr i_ref(const i_ref& source_ref_) noexcept
            {
                Type* source = source_ref_._ref;
                if (source)
                { source->IaddRef(); }
                _ref = source;
            }

            //Copy from reference.
            i_ref& operator=(const i_ref& other_)
            {
                Type* source = other_._ref;
                Type* old = _ref;
                if (source)
                { source->IaddRef(); }
                _ref = source;
                if (old)
                { old->Irelease(); }  //decrement reference and delete object if refcount == 0
                return *this;
            }

            void swap(i_ref& other_) noexcept
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
                _ref->Irelease();
                _ref = nullptr;
            }
            //This returns a pointer to the underlying object. Use with caution!

            [[deprecated]] constexpr Type* getRef() const noexcept { return _ref; }

            ///This returns a pointer to the underlying object. Use with caution!
            constexpr Type* get() const noexcept { return _ref; }

            constexpr Type* operator->() const noexcept { return _ref; }

            operator Type*() const noexcept { return _ref; }

            bool operator!=(const mafia::game_types::Ref<Type>& other_) const noexcept { return _ref != other_._ref; }

            size_t ref_count() const noexcept { return _ref ? _ref->ref_count() : 0; }
        };
    }

    class DebugValue
    {
    public:
        typedef _private::i_ref<DebugValue> RefType;

        DebugValue() = default;
        virtual ~DebugValue() = default;
        // IDebugValue
        virtual int IaddRef() = 0;
        virtual int Irelease() = 0;

        //ArmaDebugEngine
        virtual void getTypeStr(char* buffer, int len) const {};

        //base is number base. So base10 by default probably
        virtual void getValue(unsigned int base, char* buffer, int len) const {};

        virtual bool isArray() const { return false; };

        virtual int itemCount() const { return 0; };

        virtual RefType getItem(int i) const { return nullptr; };
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_DEBUG_VALUE_H
