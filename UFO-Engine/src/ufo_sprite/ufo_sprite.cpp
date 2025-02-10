#include "ufo_sprite.h"

ufoSprite::ufoSprite(
    std::string _key,
    olc::vf2d _position,
    olc::vf2d _frame_size,
    olc::vf2d _offset,
    float _scale,
    float _rotation,
    int _z
) :
key{_key},
position{_position},
parent_position{&position},
frame_size{_frame_size},
offset{_offset},
scale{_scale},
rotation{_rotation},
z{_z}
{}

void
ufoSprite::SetParentPosition(olc::vf2d* _parent_position){
    parent_position = _parent_position;
}