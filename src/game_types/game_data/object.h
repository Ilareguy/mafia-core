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
        Object() noexcept;

        size_t hash() const
        {
            return _private::pairhash(
                    type_def, reinterpret_cast<uintptr_t>(object
                                                          ? object->object
                                                          : object));
        }

        struct visualState
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
        struct visual_head_pos
        {
            bool valid {false};
            vector3 _cameraPositionWorld;
            vector3 _aimingPositionWorld;
        };
#ifndef __linux__

        //Not compatible yet. Not sure if ever will be.
        visualState get_position_matrix() const noexcept
        {
            if (!object || !object->object)
            { return visualState(); }
            const uintptr_t vbase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(object->object) +
                                                                  0xA0);

            struct visState1
            {
                vector3 _aside;
                vector3 _up;
                vector3 _dir;

                vector3 _position;
                float _scale;
                float _maxScale;
            };
            struct visState2
            {
                float _deltaT;
                vector3 _speed;
                vector3 _modelSpeed;
                vector3 _acceleration;
            };

            const auto s1 = reinterpret_cast<const visState1*>(vbase + sizeof(uintptr_t));
            const auto s2 = reinterpret_cast<const visState2*>(vbase + 0x44);

            return visualState {
                    true,
                    s1->_aside,
                    s1->_up,
                    s1->_dir,
                    {s1->_position.x, s1->_position.z, s1->_position.y},
                    s1->_scale,
                    s1->_maxScale,
                    s2->_deltaT,
                    s2->_speed,
                    s2->_modelSpeed,
                    s2->_acceleration
            };
        }

        visual_head_pos get_head_pos() const
        {
            if (!object || !object->object)
            { return visual_head_pos(); }
#if _WIN64 || __X86_64__
            uintptr_t vbase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(object->object) + 0xD0);
#else
            const auto vbase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(object->object) + 0xA0);
#endif
            class v1
            {
                virtual void doStuff() noexcept {}
            };
            class v2: public v1
            {
                void doStuff() noexcept override {}
            };
            v2* v = reinterpret_cast<v2*>(vbase);
            auto& typex = typeid(*v);
#ifdef __GNUC__
            auto test = typex.name();
#else
            const auto test = typex.raw_name();
#endif

            const auto hash = typex.hash_code();
            if (hash !=
                #if _WIN64 || __X86_64__
                0xb57aedbe2fc8b61e
                #else
                0x6d4f3e40
                #endif
                && strcmp(test, ".?AVManVisualState@@") != 0)
            {
                return visual_head_pos();
            }
            const auto s3 = reinterpret_cast<const visual_head_pos*>(vbase +
                                                                     #if _WIN64 || __X86_64__
                                                                     0x168
#else
                    0x114
#endif
            );
            return visual_head_pos();
            return visual_head_pos {
                    true,
                    {s3->_cameraPositionWorld.x, s3->_cameraPositionWorld.z, s3->_cameraPositionWorld.y},
                    {
                            s3->_aimingPositionWorld.x,
                            s3->_aimingPositionWorld.z,
                            s3->_aimingPositionWorld.y
                    }};
        }

#endif
        struct
        {
            uint32_t _x {};
            void* object {};
        } * object {};
    };
}

#endif // DEF_MAFIA_CORE_GAME_TYPES_GAME_DATA_OBJECT_H
