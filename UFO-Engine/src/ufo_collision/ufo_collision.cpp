#include "../external/olcPixelGameEngine.h"
#include "ufo_collision.h"
#include "../shapes/ray2.h"

namespace ufoMaths{

CollisionData
DynamicCircleVsRay(CollisionBody<Circle>& _circle, _velocity, Ray2 _ray){
    olc::vf2d max_distance_to_ray = _circle.radius * _ray.Normal();
    Ray2 new_ray = Ray2(_ray - max_distance_to_ray);

    return ufoMaths::RayVsRay(_circle, _velocity, _ray);
}

olc::vf2d GetDeflectionVector(olc::vf2d _position, olc::vd2d _velocity, Ray2 _plane){

    olc::vf2d deflection_vector;

    Ray2 ray_that_penetrates_slope_at_90_degrees = Ray2(_position+_velocity, _position+_velocity + olc::vf2d(_plane.Size().y, -_plane.Size().x));

    deflection_vector = (ray_that_penetrates_slope_at_90_degrees.Size() *
                            ufoMaths::RayVsRay(ray_that_penetrates_slope_at_90_degrees, _plane).collision_time);

    return deflection_vector;
}

}