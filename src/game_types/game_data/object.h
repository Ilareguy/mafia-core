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

#ifndef DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_OBJECT_H
#define DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_OBJECT_H

#include "../game_data.h"
#include "../debug_value.h"

namespace mafia::game_types::game_data
{
    class Object: public mafia::game_types::GameData
    {
    public:
        static uintptr_t type_def;
        static uintptr_t data_type_def;

    public:
        struct VisualState
        {
            //will be false if you called stuff on nullObj
            bool valid {false};
            vector3 _aside;
            vector3 _up;
            vector3 _dir;

            vector3 _position;
            float _scale;
            float _maxScale;

            float _deltaT;
            vector3 _speed;       // speed in world coordinates
            vector3 _modelSpeed;  // speed in model coordinates (updated in Move())
            vector3 _acceleration;
        };

        struct VisualHeadPosition
        {
            bool valid {false};
            vector3 _cameraPositionWorld;
            vector3 _aimingPositionWorld;
        };

    public:
        Object() noexcept;
        size_t hash() const;


#ifndef __linux__

        //Not compatible yet. Not sure if ever will be.
        VisualState get_position_matrix() const noexcept;
        VisualHeadPosition get_head_pos() const;

#endif

    public:
        struct
        {
            uint32_t _x {};
            void* object {};
        } * object {};
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_OBJECT_H
