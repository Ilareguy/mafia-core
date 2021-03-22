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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_SERIALIZE_CLASS_H
#define DEF_MAFIA_CORE_GAME_TYPES_SERIALIZE_CLASS_H

#include "rv_string.h"

namespace mafia::game_types
{
    class ParamArchive;

    enum class SerializationReturn
    {
        no_error,
        no_such_file,
        bad_file,
        bad_file_structure,
        unsupported_format,
        version_too_new,
        version_too_old,
        no_such_file2,
        access_denied,
        disk_error,
        no_entry,
        unknown_error = 12
    };

    class SerializeClass
    {
        //That's from gameValue RTTI. I'd name it serializable_class but I think keeping it close to engine name is better.
    public:
        virtual ~SerializeClass() = default;

        virtual bool _dummy(void*) { return false; }  //Probably canSerialize?
        virtual SerializationReturn serialize(ParamArchive&) { return SerializationReturn::unknown_error; }

        virtual void _dummy2(void*) {}
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_SERIALIZE_CLASS_H
