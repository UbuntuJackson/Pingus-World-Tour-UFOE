#ifndef COLLISION_SHAPE
#define COLLISION_SHAPE

#include "../external/olcPixelGameEngine.h"

class CollisionShape{
public:
    olc::vf2d velocity;
    CollisionShape();
    virtual void HandleUpdate();
    virtual void IsOverlappingOther();
};

#endif