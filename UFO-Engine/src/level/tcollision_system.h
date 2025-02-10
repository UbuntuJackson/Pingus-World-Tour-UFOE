//This file is deprecated

#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H
#include <vector>
#include "../shapes/circle.h"
#include "../ufo_collision/collision_data.h"
#include "../shapes/ray2.h"
#include "../json/json.h"

template<typename tActor>
class TCollisionBody;

class Camera;

class TCollisionSystem{
public:
    std::vector<Ray2> lines;
    std::vector<Ray2> modified_lines;
    int collision_search_depth = 0;
    int counter = 0;
    bool should_move_with_delta_time = false;

    void Load(Json& _json);

    void ResolveCircleVsLine(TCollisionBody<Circle>* _collision_body);

    void UpdateCollisionBody(TCollisionBody<Circle>* _collision_body);

    Ray2 GetRayOffsetByCircleRadius(Circle _circle, olc::vf2d _velocity, Ray2 _ray);
    
    olc::vf2d
    DynamicCircleVsRay(Circle _circle, olc::vf2d _velocity, Ray2 _ray);

    olc::vf2d GetDeflectionVector(olc::vf2d _position, olc::vd2d _velocity, Ray2 _plane);

    Ray2 CreateLineFromCircleTangent(Circle _dynamic_circle, Circle _static_circle);

    void Draw(Camera* _camera);

};

#endif