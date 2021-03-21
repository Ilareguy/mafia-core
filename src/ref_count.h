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

#ifndef DEF_MAFIA_CORE_REF_COUNT_H
#define DEF_MAFIA_CORE_REF_COUNT_H

namespace mafia
{
    class RefCountBase
    {
    public:
        constexpr RefCountBase() noexcept: _refcount(0) {}

        constexpr RefCountBase(const RefCountBase&) noexcept: _refcount(0) {}

        constexpr void operator=(const RefCountBase&) const noexcept {}

        constexpr int add_ref() const noexcept
        {
            return ++_refcount;
        }

        constexpr int dec_ref() const noexcept
        {
            return --_refcount;
        }

        constexpr int ref_count() const noexcept
        {
            return _refcount;
        }

        mutable int _refcount;
    };

    // RefCount has to be the first element in a class. Use refcount_vtable instead if refcount is preceded by a vtable
    class RefCount: public RefCountBase
    {
    public:
        virtual ~RefCount() = default;

        int release() const
        {
            const auto rcount = dec_ref();
            if (rcount == 0)
            {
                //this->~refcount();
                lastRefDeleted();
                //rv_allocator<refcount>::deallocate(const_cast<refcount *>(this), 0);
            }
            return rcount;
        }

        void destruct() const noexcept
        {
            delete const_cast<RefCount*>(this);
        }

        virtual void lastRefDeleted() const { destruct(); }

    private:
        virtual int _dummy_refcount_func() const noexcept { return 0; }
    };
}

#endif //DEF_MAFIA_CORE_REF_COUNT_H
