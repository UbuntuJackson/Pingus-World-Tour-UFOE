#ifndef ALTERNATIVE_COLLISION_SYSTEM_H
#define ALTERNATIVE_COLLISION_SYSTEM_H
#include <vector>
#include "../shapes/circle.h"
#include "../ufo_collision/collision_data.h"
#include "../shapes/ray2.h"
#include "../json/json.h"

class CollisionBody;
class CollisionCircle;
class CollisionPill;

class Camera;

class CollisionSystem{
public:
    std::vector<Ray2> lines;
    std::vector<Ray2> modified_lines;
    int collision_search_depth = 0;
    int counter = 0;
    bool should_move_with_delta_time = false;

    void Load(Json& _json);

    //Ray2 CreateLineFromCircleCollision(Circle _dynamic_circle, olc::vf2d _velocity, Ray2 _line);

    void ResolveCircleVsLine(CollisionCircle* _collision_body, olc::vf2d& _velocity);

    void UpdateCollisionBody(CollisionBody* _collision_body);

    Ray2 GetRayOffsetByCircleRadius(Circle _circle, olc::vf2d _velocity, Ray2 _ray);
    
    olc::vf2d
    DynamicCircleVsRay(Circle _circle, olc::vf2d _velocity, Ray2 _ray);

    olc::vf2d GetDeflectionVector(olc::vf2d _position, olc::vd2d _velocity, Ray2 _plane);

    Ray2 CreateLineFromCircleTangent(Circle _dynamic_circle, Circle _static_circle);

    void ResolvePillVsLine(CollisionPill* _pill, olc::vf2d& _velocity);

    void ResolveCircleVsLine202412(CollisionCircle* _circle, CollisionBody* _collision_body, olc::vf2d& _velocity, std::vector<std::pair<float, Ray2>>& hit_line_segments);

    void Draw(Camera* _camera);

};

#endif