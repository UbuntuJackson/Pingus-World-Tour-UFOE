#ifndef ANIMATED_SPRITE_REFERENCE
#define ANIMATED_SPRITE_REFERENCE

#include "../sprite_reference/sprite_reference.h"

class AnimatedSpriteReference : public SpriteReference{
public:
    float pace;
    AnimatedSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, float _pace, bool _temporary = false);
};

#endif