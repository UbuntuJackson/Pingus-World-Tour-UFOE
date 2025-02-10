#ifndef UFO_RAY_H
#define UFO_RAY_H
#include "../external/olcPixelGameEngine.h"
#include "../ufo_collision/collision_data.h"
template<typename tType>
struct ufoRay{
    tType ray;
    bool visible;
    olc::Pixel color;
    CollisionData collision_data;
    olc::vf2d* parent_start;
    olc::vf2d* parent_end;

    ufoRay(
        tType _shape,
        bool _visible = false,
        olc::Pixel _color = olc::WHITE
    ):
    ray{_shape},
    visible{_visible},
    color{_color}{}
    void SetParentStart(olc::vf2d* _parent_start){
        parent_start = _parent_start;
    }
    void SetParentEnd(olc::vf2d* _parent_end){
        parent_end = _parent_end;
    }
};

#endif