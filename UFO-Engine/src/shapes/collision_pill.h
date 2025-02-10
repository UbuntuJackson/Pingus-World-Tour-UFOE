#ifndef COLLISION_PILL_H
#define COLLISION_PILL_H

#include "pill.h"
#include "../external/olcPixelGameEngine.h"

class CollisionBody;

class CollisionPill : public Pill{
public:
    bool collision_happened_this_frame = false;
    CollisionBody* collision_body;
    CollisionPill(olc::vf2d _position, float _radius, float _height, CollisionBody* _collision_body);

    void Update();

};

#endif