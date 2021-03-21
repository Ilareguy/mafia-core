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

#ifndef DEF_MAFIA_CORE_UNIQUE_REF_H
#define DEF_MAFIA_CORE_UNIQUE_REF_H

namespace mafia
{
    ///When this goes out of scope. The pointer is deleted. This takes ownership of the pointer
    template<class Type>
    class UniqueRef
    {
    protected:
        Type* _ref;

    public:
        UniqueRef() { _ref = NULL; }

        UniqueRef(Type* source)
        {
            _ref = source;
        }

        UniqueRef(const UniqueRef& other)
        {
            _ref = other._ref;
            other._ref = nullptr;  //We take ownership
        }

        ~UniqueRef() { clear(); }

        UniqueRef& operator=(Type* other)
        {
            if (_ref == other)
            { return *this; }
            clear();
            _ref = other;
            return *this;
        }

        UniqueRef& operator=(const UniqueRef& other)
        {
            if (other._ref == _ref)
            { return *this; }
            clear();
            _ref = other._ref;
            other._ref = nullptr;  //We take ownership
            return *this;
        }

        bool is_null() const { return _ref == nullptr; }

        void clear()
        {
            if (_ref)
            { delete _ref; }
            _ref = nullptr;
        }

        Type* operator->() const { return _ref; }

        explicit operator Type*() const { return _ref; }

        Type* get() const { return _ref; }
    };
}

#endif // DEF_MAFIA_CORE_UNIQUE_REF_H
