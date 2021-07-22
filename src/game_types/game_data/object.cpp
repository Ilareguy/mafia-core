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

#include "object.h"
#include "../../pair_hash.h"

using namespace mafia::game_types::game_data;

uintptr_t Object::type_def {0};
uintptr_t Object::data_type_def {0};

Object::Object() noexcept
{
    *reinterpret_cast<uintptr_t*>(this) = type_def;
    *reinterpret_cast<uintptr_t*>(static_cast<mafia::game_types::DebugValue*>(this)) = data_type_def;
}

size_t Object::hash() const
{
    return mafia::pair_hash(
            type_def, reinterpret_cast<uintptr_t>(object
                                                  ? object->object
                                                  : object));
}

Object::VisualHeadPosition Object::get_head_pos() const
{
    if (!object || !object->object)
    { return VisualHeadPosition(); }
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
        return VisualHeadPosition();
    }
    const auto s3 = reinterpret_cast<const VisualHeadPosition*>(vbase +
                                                             #if _WIN64 || __X86_64__
                                                             0x168
#else
            0x114
#endif
    );
    return VisualHeadPosition();
    return VisualHeadPosition {
            true,
            {s3->_cameraPositionWorld.x, s3->_cameraPositionWorld.z, s3->_cameraPositionWorld.y},
            {
                    s3->_aimingPositionWorld.x,
                    s3->_aimingPositionWorld.z,
                    s3->_aimingPositionWorld.y
            }};
}

Object::VisualState Object::get_position_matrix() const noexcept
{
    if (!object || !object->object)
    { return VisualState(); }
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

    return VisualState {
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
