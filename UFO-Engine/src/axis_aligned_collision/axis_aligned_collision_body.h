#ifndef AXIS_ALIGNED_COLLISION_BODY
#define AXIS_ALIGNED_COLLISION_BODY

#include "../shapes/rectangle.h"
#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"

class AxisAlignedCollisionBody : public Actor{
public:
    Rectangle shape;
    olc::vf2d velocity = {0.0f, 0.0f};
    olc::vf2d acceleration = {1.0f, 1.0f};
    AxisAlignedCollisionBody(olc::vf2d _position, Rectangle _shape);
    void OnUpdate();
};

#endif