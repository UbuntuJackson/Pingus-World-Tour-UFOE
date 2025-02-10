#ifndef DYNAMIC_SHAPE_H
#define DYNAMIC_SHAPE_H
#include "../external/olcPixelGameEngine.h"

template<typename tType>
struct DynamicShape{
    tType shape;
    olc::vf2d* parent_velocity;
    olc::vf2d velocity;
    olc::vf2d* parent_position;
    bool visible;
    DynamicShape(
        tType _shape,
        olc::vf2d _velocity,
        bool _visible = false
    ):
    shape{_shape},
    parent_position{&(_shape.position)},
    velocity{_velocity},
    parent_velocity{&_velocity},
    visible{_visible}{}

    void SetParentPosition(olc::vf2d* _parent_position){
        parent_position = _parent_position;
    }
};

#endif