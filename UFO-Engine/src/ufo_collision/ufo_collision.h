#include "../ufo_maths/ufo_maths.h"
#include "../shapes/ray2.h"
#include "../external/olcPixelGameEngine.h"
#include "collision_data.h"

namespace ufoMaths{

CollisionData
DynamicCircleVsRay(DynamicCircle& _circle, _velocity, Ray2 _ray);

olc::vf2d GetDeflectionVector(olc::vf2d _position, olc::vd2d _velocity, Ray2 _plane);

}