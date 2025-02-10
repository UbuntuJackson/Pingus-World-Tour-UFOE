#ifndef UFO_SHAPE_H
#define UFO_SHAPE_H
#include "../external/olcPixelGameEngine.h"
template<typename tType>
struct ufoShape{
    tType shape;
    olc::vf2d* parent_position;
    bool visible;
    ufoShape(
        tType _shape,
        bool _visible = false
    ):
    shape{_shape},
    parent_position{&(_shape.position)},
    visible{_visible}{}

    void SetParentPosition(olc::vf2d* _parent_position){
        parent_position = _parent_position;
    }
};

#endif