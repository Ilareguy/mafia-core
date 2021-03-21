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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_SOURCE_DOC_H
#define DEF_MAFIA_CORE_GAME_TYPES_SOURCE_DOC_H

#include "serialization.h"
#include "./string.h"

namespace mafia::game_types
{
    class ParamArchive;

    struct SourceDoc : public SerializeClass {  //See ArmaDebugEngine for more info on this
        String sourcefile;
        String content;

        SerializationReturn serialize(ParamArchive& ar) override;
    };

    struct SourceDocPosition : public SerializeClass {  //See ArmaDebugEngine for more info on this
        String sourcefile;
        uint32_t sourceline;
        String content;
        uint32_t pos;

        SerializationReturn serialize(ParamArchive& ar) override;
    };

    struct SourceDocPositionRef : public SourceDocPosition, RefCount {};
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_SOURCE_DOC_H
