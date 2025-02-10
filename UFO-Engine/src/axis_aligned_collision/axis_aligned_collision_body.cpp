#include "../shapes/rectangle.h"
#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "axis_aligned_collision_body.h"

AxisAlignedCollisionBody::AxisAlignedCollisionBody(olc::vf2d _position, Rectangle _shape) :
    Actor(_position),
    shape{_shape}
{

}

void AxisAlignedCollisionBody::OnUpdate(){
    shape.position = position;
}