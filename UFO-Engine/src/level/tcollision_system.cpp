//This file is deprecated

#include <vector>
#include <algorithm>
#include <utility>
#include "tcollision_system.h"
#include "../shapes/circle.h"
#include "../ufo_engine/ufo_engine.h"
#include "../console/console.h"
#include "tcollision_body.h"
#include "../ufo_collision/collision_data.h"
#include "../shapes/ray2.h"
#include "../json/json.h"
#include "../drawing_system/drawing_system.h"
#include "../camera/camera.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_maths/is_polygon_clockwise.h"

void TCollisionSystem::Load(Json& _json){
    lines.clear();
    Json objects = _json.GetObject("objects");
    Json::ArrayForEach(Json(), objects, [this](Json _object, Json _objects){
        std::vector<olc::vf2d> points;
        Json polygon = _object.GetObject("polygon");
        float global_x = (float)_object.GetAsInt("x");
        float global_y = (float)_object.GetAsInt("y");
        Json::ArrayForEach(Json(), polygon, [&](Json _point, Json _polygon){
            float x = (float)_point.GetAsInt("x");
            float y = (float)_point.GetAsInt("y");
            points.push_back(olc::vf2d(global_x, global_y)+olc::vf2d(x,y));
        });
        if(ufoMaths::IsPolygonClockwise(points)){
            for(int i = 0; i < points.size(); i++){
                Ray2 line = Ray2(points[i], points[(i+1)%points.size()]);
                lines.push_back(line);
            }
        }
        else{
            for(int i = points.size()-1; i >= 0; i--){
                Ray2 line = Ray2(points[i], points[(i-1)%points.size()]);
                lines.push_back(line);
            }
        }
    });

}

void TCollisionSystem::ResolveCircleVsLine(TCollisionBody<Circle>* _collision_body){
    collision_search_depth++;
    //if(collision_search_depth > 0) Console::Out(collision_search_depth);
    if(collision_search_depth > 30){
        Console::Out("maximum recursion depth was reached");
        return;
    }

    if(_collision_body->velocity.mag() == 0.0f) return;

    std::vector<std::pair<float, Ray2>> hit_line_segments;
    float radius = _collision_body->shape.radius;
    olc::vf2d velocity_delta_time;
    if(Engine::Get().vsync) velocity_delta_time = _collision_body->velocity;
    else velocity_delta_time = _collision_body->velocity * Engine::Get().GetDeltaTime();
    
    for(const auto& line : lines){
        Ray2 line_from_corner = CreateLineFromCircleTangent(Circle(_collision_body->shape.position, 0.0f), Circle(line.Start(), _collision_body->shape.radius));
        Ray2 new_ray = GetRayOffsetByCircleRadius(_collision_body->shape, velocity_delta_time, line_from_corner);
        modified_lines.push_back(new_ray);
        CollisionData collision_data = ufoMaths::RayVsRay(Ray2(_collision_body->local_position, _collision_body->local_position+velocity_delta_time), new_ray);

        //if(collision_data.cross_product_z_component == 0.0f){
        //    Console::Out("z component was 0",counter++, collision_data.collision_time);
        //    _collision_body->velocity-=new_ray.Normal().norm();
        //    collision_data = ufoMaths::RayVsRay(Ray2(_collision_body->position, _collision_body->position+velocity_delta_time), new_ray);
        //}

        if(collision_data.is_hit){
            //Console::Out("Dot product:",ufoMaths::DotProduct(new_ray.Normal(), velocity_delta_time));
            //Console::Out("Ray before adding top check new ray:", new_ray.Start(), new_ray.End());
            hit_line_segments.push_back(std::make_pair(collision_data.collision_time,line_from_corner));

            Ray2 check_new_ray = GetRayOffsetByCircleRadius(_collision_body->shape, velocity_delta_time, hit_line_segments.back().second);
            //Console::Out("Ray added to hit line segments:", check_new_ray.Start(), check_new_ray.End());
            //Console::Out("Corner hit");
        }
    }

    for(const auto& line : lines){
        Ray2 new_ray = GetRayOffsetByCircleRadius(_collision_body->shape, velocity_delta_time, line);
        modified_lines.push_back(new_ray);
        CollisionData collision_data = ufoMaths::RayVsRay(Ray2(_collision_body->local_position, _collision_body->local_position+velocity_delta_time), new_ray);

        if(collision_data.is_hit){
            hit_line_segments.push_back(std::make_pair(collision_data.collision_time,line));
        }
    }
    std::sort(hit_line_segments.begin(), hit_line_segments.end(),[&](const auto _a, const auto _b){return _b.first < _a.first;});

    
    //if(hit_line_segments.size() > 0) Console::Out("hit line segments was", hit_line_segments.size(), counter++);
    for(const auto& line : hit_line_segments){
        _collision_body->collision_normal += line.second.Normal().norm();
        _collision_body->collision_normal.norm();
        //Console::Out("Velocity before resolving:", velocity_delta_time);
        _collision_body->velocity = DynamicCircleVsRay(_collision_body->shape, velocity_delta_time, line.second);
        //Console::Out("Velocity after resolving:", _collision_body->velocity);
        ResolveCircleVsLine(_collision_body);
    }
}

void TCollisionSystem::UpdateCollisionBody(TCollisionBody<Circle>* _collision_body){
    _collision_body->Accelerate(); //just accelerates, presumably.
    _collision_body->collision_normal = {0.0f, 0.0f};
    
    ResolveCircleVsLine(_collision_body);
    _collision_body->OnBeforeMove();
    _collision_body->Move(); //just moves with respect to delta time, presumably.
    collision_search_depth = 0;
}

Ray2 TCollisionSystem::GetRayOffsetByCircleRadius(Circle _circle, olc::vf2d _velocity, Ray2 _ray){
    float radius = _circle.radius;
    olc::vf2d max_distance_to_ray = radius * _ray.Normal().norm();
    olc::vf2d extended_segment_with_radius = (_ray.p1-_ray.p0).norm()*radius;
    Ray2 new_ray = Ray2(_ray.p0 + max_distance_to_ray /*- extended_segment_with_radius*/, _ray.p1 + max_distance_to_ray /*+ extended_segment_with_radius*/);
    return new_ray;
}

olc::vf2d
TCollisionSystem::DynamicCircleVsRay(Circle _circle, olc::vf2d _velocity, Ray2 _ray){
    Ray2 new_ray = GetRayOffsetByCircleRadius(_circle, _velocity, _ray);

    //Console::Out("Ray:", new_ray.Start(), new_ray.End());

    CollisionData collision_data = ufoMaths::RayVsRay(Ray2(_circle.position, _circle.position+_velocity), new_ray);

    if(!collision_data.is_hit) return _velocity;

    if(new_ray.Size().mag() < 5.0f){
        Console::Out("Velocity", _velocity.norm());
        Console::Out("Line was:", new_ray.Size().norm(), new_ray.Normal().norm());
        Console::Out(collision_data.cross_product_z_component);
        Console::Out(collision_data.other_cross_product_z_component);
    }
    if(collision_data.cross_product_z_component == 0.0f) Console::Out("z component was 0",counter++, collision_data.collision_time);
    if(collision_data.other_cross_product_z_component == 0.0f) Console::Out("other z component was 0",counter++, collision_data.other_collision_time);

    olc::vf2d deflection_vector = GetDeflectionVector(_circle.position, _velocity, new_ray);
    _velocity += (deflection_vector + deflection_vector.norm()*0.01f);
    return _velocity;
}

olc::vf2d
TCollisionSystem::GetDeflectionVector(
    olc::vf2d _position, //position for intersection resulting from DynamicCircleVsRay
    olc::vd2d _velocity,
    Ray2 _plane){

    olc::vf2d deflection_vector;

    Ray2 ray_that_penetrates_slope_at_90_degrees = Ray2(_position+_velocity, _position+_velocity + _plane.Normal().norm());

    deflection_vector = (ray_that_penetrates_slope_at_90_degrees.Size() *(
                            ufoMaths::RayVsRay(ray_that_penetrates_slope_at_90_degrees, _plane).collision_time));
    //Console::Out("Deflection vector:", deflection_vector);
    return deflection_vector;
}

Ray2 TCollisionSystem::CreateLineFromCircleTangent(Circle _dynamic_circle, Circle _static_circle){
    olc::vf2d distance_vector = _static_circle.position - _dynamic_circle.position;
    olc::vf2d static_circle_radius_as_vector = distance_vector.norm() * _static_circle.radius;

    //make static radius as vector into ray and take the normal.
    olc::vf2d static_circle_tangent = Ray2({0.0f, 0.0f}, static_circle_radius_as_vector).Normal();

    //I suggest this to be the line that is tested
    //Ray2 line = Ray2(_static_circle.position - static_circle_tangent.norm(), _static_circle.position + static_circle_tangent.norm());
    return Ray2(_static_circle.position - static_circle_tangent.norm()*0.5f, _static_circle.position + static_circle_tangent.norm()*0.5f); 
}

void TCollisionSystem::Draw(Camera* _camera){
    if(!Engine::Get().all_shapes_visible) return;
    for(auto line : lines){
        DrawingSystem::Draw(line, _camera, olc::MAGENTA);
        olc::vf2d middle_position = line.Start()+(line.End()-line.Start())/2.0f;
        DrawingSystem::Draw(Ray2(middle_position, middle_position+line.Normal().norm()*5.0f), _camera, olc::MAGENTA);
    }
    for(auto line : modified_lines){
        DrawingSystem::Draw(line, _camera, olc::Pixel(255, 0, 0, 40));
        olc::vf2d middle_position = line.Start()+(line.End()-line.Start())/2.0f;
        DrawingSystem::Draw(Ray2(middle_position, middle_position+line.Normal().norm()*5.0f), _camera, olc::Pixel(255, 0, 0, 40));
    }
    modified_lines.clear();
}