#include <vector>
#include <algorithm>
#include <utility>
#include "../external/olcPixelGameEngine.h"
#include "collision_system.h"
#include "../shapes/circle.h"
#include "../ufo_engine/ufo_engine.h"
#include "../console/console.h"
#include "../ufo_collision/collision_data.h"
#include "../shapes/ray2.h"
#include "../json/json_variant.h"
#include "../drawing_system/drawing_system.h"
#include "../camera/camera.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_maths/is_polygon_clockwise.h"
#include "collision_body.h"
#include "../shapes/collision_circle.h"
#include "../shapes/collision_pill.h"
#include "../ufo_vector_utils/find_in_vector.h"

void CollisionSystem::Load(JsonDictionary& _json){
    lines.clear();
    JsonArray& objects = _json.Get("objects").AsArray();
    for(auto&& _object : objects.Iterable()){
        auto object = _object->AsDictionary();
        std::vector<olc::vf2d> points;
        JsonArray polygon = object.Get("polygon").AsArray();
        float global_x = object.Get("x").AsFloat();
        float global_y = object.Get("y").AsFloat();
        for(auto&& _point : polygon.Iterable()){
            float x = _point->AsDictionary().Get("x").AsFloat();
            float y = _point->AsDictionary().Get("y").AsFloat();
            points.push_back(olc::vf2d(global_x, global_y)+olc::vf2d(x,y));
        }
        if(ufoMaths::IsPolygonClockwise(points)){
            if(object.Get("name").AsString() != "") Console::Out("Loading clockwise polygon with name:",object.Get("name").AsString());

            for(int i = 0; i < points.size(); i++){
                Ray2 line = Ray2(points[i], points[(i+1)%points.size()]);
                lines.push_back(line);
            }
        }
        else{
            if(object.Get("name").AsString() != "") Console::Out("Loading counter clockwise polygon with name:",object.Get("name").AsString());

            for(int i = points.size()-1; i >= 0; i--){
                Ray2 line = Ray2(points[(i+1)%points.size()],points[i]);
                lines.push_back(line);
            }
        }
    }

}

/*Ray2 CollisionSystem::CreateLineFromCircleCollision(Circle _dynamic_circle, olc::vf2d _velocity, Ray2 _line){
    Shape<Circle> dynamic_circle = Shape<Circle>(Circle(olc::vf2d(0.0f, 0.0f),_dynamic_circle.radius), _line.Start());
    
    RayVsCircleCollisionData result = ufoMaths::RayVsCircle(Ray2(_dynamic_circle.position, _dynamic_circle.position + _velocity), &dynamic_circle);

    if(result.is_hit){
        olc::vf2d line_size = Ray2(olc::vf2d(0.0f, 0.0f),result.collision_normal).Normal();
    }

}*/

void CollisionSystem::ResolveCircleVsLine(CollisionCircle* _collision_shape, olc::vf2d& _velocity){
    //if(collision_search_depth > 0) Console::Out(collision_search_depth);
    collision_search_depth++;
    
    if(collision_search_depth > 50){
        Console::Out("maximum recursion depth was reached");
        return;
    }

    if(_velocity.mag() == 0.0f) return;

    std::vector<std::pair<float, Ray2>> hit_line_segments;
    olc::vf2d velocity_delta_time;
    velocity_delta_time = _velocity;
    
    for(int line_index = 0; line_index < lines.size(); line_index++){

        //Make line out of circle
        Shape<Circle> dynamic_circle = Shape<Circle>(Circle(olc::vf2d(0.0f, 0.0f),_collision_shape->radius), lines[line_index].Start());
    
        dynamic_circle.position = dynamic_circle.local_position;

        RayVsCircleCollisionData result = ufoMaths::RayVsCircle(Ray2(_collision_shape->position, _collision_shape->position + velocity_delta_time), &dynamic_circle);

        //Console::Out(dynamic_circle.position, dynamic_circle.local_position);

        if(result.is_hit){

            olc::vf2d line_size = Ray2(olc::vf2d(0.0f, 0.0f),result.collision_normal).Normal()*-10.0f;

            Ray2 corner_line = Ray2(lines[line_index].Start() - line_size, lines[line_index].Start() + line_size);

            //Getting the new line for drawing purposes.
            Ray2 new_ray = GetRayOffsetByCircleRadius(*_collision_shape, velocity_delta_time, corner_line);
            modified_lines.push_back(new_ray);
            hit_line_segments.push_back(std::make_pair(result.collision_time,corner_line));

        }
    }

    for(const auto& line : lines){
        Ray2 new_ray = GetRayOffsetByCircleRadius(*_collision_shape, velocity_delta_time, line);
        modified_lines.push_back(new_ray);
        CollisionData collision_data = ufoMaths::RayVsRay(Ray2(_collision_shape->position, _collision_shape->position+velocity_delta_time), new_ray);

        if(collision_data.is_hit){
            hit_line_segments.push_back(std::make_pair(collision_data.collision_time,line));
        }
    }

    //If a is less than b, swap.
    std::sort(hit_line_segments.begin(), hit_line_segments.end(),[&](const auto _a, const auto _b){return _b.first > _a.first;});

    if(hit_line_segments.size() > 0) _collision_shape->collision_happened_this_frame = true;
    //if(hit_line_segments.size() > 0) Console::Out("hit line segments was", hit_line_segments.size(), counter++);
    for(const auto& line : hit_line_segments){
        if(collision_search_depth <= 2) _collision_shape->collision_body->collision_normal += line.second.Normal().norm();
        //_collision_shape->collision_body->collision_normal = _collision_shape->collision_body->collision_normal.norm();
        
        //Would be good if this function could be some sort of black box that calculates the new velocity for respective shape.
        _velocity = DynamicCircleVsRay(*_collision_shape, velocity_delta_time, line.second);

        //Checking if the adjusted velocity vector crosses the line

        //_collision_shape->position+=line.second.Normal().norm()*0.01f;

        /*CollisionData data = ufoMaths::RayVsRay(Ray2(_collision_shape->position, _collision_shape->position+_velocity), GetRayOffsetByCircleRadius(*_collision_shape, velocity_delta_time, line.second));
        if(data.is_hit){
            _collision_shape->position = data.intersection_point - line.second.Normal().norm()*1.0f;
        }*/

        ResolveCircleVsLine(_collision_shape, _velocity);
        break;
    }
}

void CollisionSystem::UpdateCollisionBody(CollisionBody* _collision_body){
    _collision_body->Accelerate(); //just accelerates, presumably.
    _collision_body->collision_normal = {0.0f, 0.0f};
    
    _collision_body->shape->Update();

    //ResolveCircleVsLine(_collision_body);
    _collision_body->OnBeforeMove();
    _collision_body->Move(); //just moves with respect to delta time, presumably.
    collision_search_depth = 0;
}

Ray2 CollisionSystem::GetRayOffsetByCircleRadius(Circle _circle, olc::vf2d _velocity, Ray2 _ray){
    float radius = _circle.radius;
    olc::vf2d max_distance_to_ray = radius * _ray.Normal().norm();
    olc::vf2d extended_segment_with_radius = (_ray.p1-_ray.p0).norm()*radius;
    Ray2 new_ray = Ray2(_ray.p0 + max_distance_to_ray /*- extended_segment_with_radius*/, _ray.p1 + max_distance_to_ray /*+ extended_segment_with_radius*/);
    return new_ray;
}

olc::vf2d
CollisionSystem::DynamicCircleVsRay(Circle _circle, olc::vf2d _velocity, Ray2 _ray){
    Ray2 new_ray = GetRayOffsetByCircleRadius(_circle, _velocity, _ray);

    //Console::Out("Ray:", new_ray.Start(), new_ray.End());

    CollisionData collision_data = ufoMaths::RayVsRay(Ray2(_circle.position, _circle.position+_velocity), new_ray);

    if(!collision_data.is_hit) return _velocity;

    if(collision_data.cross_product_z_component == 0.0f) Console::Out("z component was 0",counter++, collision_data.collision_time);
    if(collision_data.other_cross_product_z_component == 0.0f) Console::Out("other z component was 0",counter++, collision_data.other_collision_time);

    olc::vf2d deflection_vector = GetDeflectionVector(_circle.position, _velocity, new_ray);
    _velocity += (deflection_vector /*+ deflection_vector.norm()*0.01f*/);
    return _velocity;
}

olc::vf2d
CollisionSystem::GetDeflectionVector(
    olc::vf2d _position, //position for intersection resulting from DynamicCircleVsRay
    olc::vd2d _velocity,
    Ray2 _plane)
{

    olc::vf2d deflection_vector;

    Ray2 ray_that_penetrates_slope_at_90_degrees = Ray2(_position+_velocity, _position+_velocity + _plane.Normal().norm());

    deflection_vector = (ray_that_penetrates_slope_at_90_degrees.Size() *(
                            ufoMaths::RayVsRay(ray_that_penetrates_slope_at_90_degrees, _plane).collision_time));
    //Console::Out("Deflection vector:", deflection_vector);
    return deflection_vector;
}

//Deprecated
Ray2 CollisionSystem::CreateLineFromCircleTangent(Circle _dynamic_circle, Circle _static_circle){
    olc::vf2d distance_vector = _static_circle.position - _dynamic_circle.position;
    olc::vf2d static_circle_radius_as_vector = distance_vector.norm() * _static_circle.radius;

    //make static radius as vector into ray and take the normal.
    olc::vf2d static_circle_tangent = Ray2({0.0f, 0.0f}, static_circle_radius_as_vector).Normal();

    //I suggest this to be the line that is tested
    //Ray2 line = Ray2(_static_circle.position - static_circle_tangent.norm(), _static_circle.position + static_circle_tangent.norm());
    return Ray2(_static_circle.position - static_circle_tangent.norm()*0.5f, _static_circle.position + static_circle_tangent.norm()*0.5f); 
}


void CollisionSystem::ResolvePillVsLine(CollisionPill* _pill, olc::vf2d& _velocity){

    collision_search_depth++;
    //if(collision_search_depth > 0) Console::Out(collision_search_depth);
    if(collision_search_depth > 50){
        Console::Out("maximum recursion depth was reached");
        return;
    }

    if(_velocity.mag() == 0.0f) return;

    std::vector<PillResolutionData> hit_line_segments;
    olc::vf2d velocity_delta_time;
    velocity_delta_time = _velocity;
    //else velocity_delta_time = _velocity * Engine::Get().GetDeltaTime();
    
    
    //Bottom circle
    Circle bottom_circle = Circle(_pill->position+olc::vf2d(0.0f,_pill->height), _pill->radius);

    for(int line_index = 0; line_index < lines.size(); line_index++){

        //Make line out of circle
        Shape<Circle> dynamic_circle = Shape<Circle>(Circle(olc::vf2d(0.0f, 0.0f),bottom_circle.radius), lines[line_index].Start());
    
        dynamic_circle.position = dynamic_circle.local_position;

        RayVsCircleCollisionData result = ufoMaths::RayVsCircle(Ray2(bottom_circle.position, bottom_circle.position + velocity_delta_time), &dynamic_circle);

        //Console::Out(dynamic_circle.position, dynamic_circle.local_position);

        if(result.is_hit){

            olc::vf2d line_size = Ray2(olc::vf2d(0.0f, 0.0f),result.collision_normal).Normal()*-10.0f;

            Ray2 corner_line = Ray2(lines[line_index].Start() - line_size, lines[line_index].Start() + line_size);

            //Getting the new line for drawing purposes.
            Ray2 new_ray = GetRayOffsetByCircleRadius(bottom_circle, velocity_delta_time, corner_line);
            modified_lines.push_back(new_ray);
            hit_line_segments.push_back(PillResolutionData{result.collision_time,corner_line,PillCollidedSegment::BOTTOM_CIRCLE});

        }
    }

    for(const auto& line : lines){
        Ray2 new_ray = GetRayOffsetByCircleRadius(bottom_circle, velocity_delta_time, line);
        modified_lines.push_back(new_ray);
        CollisionData collision_data = ufoMaths::RayVsRay(Ray2(bottom_circle.position, bottom_circle.position+velocity_delta_time), new_ray);

        if(collision_data.is_hit){
            hit_line_segments.push_back(PillResolutionData{collision_data.collision_time,line,PillCollidedSegment::BOTTOM_CIRCLE});
        }
    }

    //Top circle
    Circle top_circle = Circle(_pill->position, _pill->radius);

    for(int line_index = 0; line_index < lines.size(); line_index++){

        //Make line out of circle
        Shape<Circle> dynamic_circle = Shape<Circle>(Circle(olc::vf2d(0.0f, 0.0f),top_circle.radius), lines[line_index].Start());
    
        dynamic_circle.position = dynamic_circle.local_position;

        RayVsCircleCollisionData result = ufoMaths::RayVsCircle(Ray2(top_circle.position, top_circle.position + velocity_delta_time), &dynamic_circle);

        //Console::Out(dynamic_circle.position, dynamic_circle.local_position);

        if(result.is_hit){

            olc::vf2d line_size = Ray2(olc::vf2d(0.0f, 0.0f),result.collision_normal).Normal()*-10.0f;

            Ray2 corner_line = Ray2(lines[line_index].Start() - line_size, lines[line_index].Start() + line_size);

            //Getting the new line for drawing purposes.
            Ray2 new_ray = GetRayOffsetByCircleRadius(top_circle, velocity_delta_time, corner_line);
            modified_lines.push_back(new_ray);
            hit_line_segments.push_back(PillResolutionData{result.collision_time,corner_line,PillCollidedSegment::TOP_CIRCLE});

        }
    }

    for(const auto& line : lines){
        Ray2 new_ray = GetRayOffsetByCircleRadius(top_circle, velocity_delta_time, line);
        modified_lines.push_back(new_ray);
        CollisionData collision_data = ufoMaths::RayVsRay(Ray2(top_circle.position, top_circle.position+velocity_delta_time), new_ray);

        if(collision_data.is_hit){
            Console::Out("Hit wall");
            hit_line_segments.push_back(PillResolutionData{collision_data.collision_time,line,PillCollidedSegment::TOP_CIRCLE});
        }
    }

    //Sides
    if(velocity_delta_time.x < 0.0f){
        Ray2 pill_left_side = Ray2(olc::vf2d(_pill->position.x-_pill->radius, _pill->position.y), olc::vf2d(_pill->position.x-_pill->radius, _pill->position.y+_pill->height));
        for(int line_index = 0; line_index < lines.size(); line_index++){
            
            //pill left side VS ray from point on line segment minus velocity vector
            CollisionData result = ufoMaths::RayVsRay(Ray2(lines[line_index].Start(), lines[line_index].Start() - velocity_delta_time), pill_left_side);
            if(result.is_hit){
                hit_line_segments.push_back(PillResolutionData{result.collision_time, lines[line_index], PillCollidedSegment::LEFT_SIDE});
                Console::Out("Hit a side");
            }
        }
    }

    if(velocity_delta_time.x > 0.0f){
        Ray2 pill_right_side = Ray2(olc::vf2d(_pill->position.x+_pill->radius, _pill->position.y), olc::vf2d(_pill->position.x+_pill->radius, _pill->position.y+_pill->height));
        for(int line_index = 0; line_index < lines.size(); line_index++){
            
            //pill left side VS ray from point on line segment plus velocity vector
            CollisionData result = ufoMaths::RayVsRay(Ray2(lines[line_index].Start(), lines[line_index].Start() - velocity_delta_time), pill_right_side);
            if(result.is_hit){
                hit_line_segments.push_back(PillResolutionData{result.collision_time, lines[line_index], PillCollidedSegment::RIGHT_SIDE});
                Console::Out("Hit a side");
            }
        }
    }

    //Check collision times for circle
    //Check collision times for other circle
    //Check collision times for lines

    //Sort collision times, reevaluate

    std::sort(hit_line_segments.begin(), hit_line_segments.end(),[&](const auto _a, const auto _b){return _b.collision_time < _a.collision_time;});

    if(hit_line_segments.size() > 0){
        _pill->collision_happened_this_frame = true;

        PillResolutionData line_segment = hit_line_segments.back();

        switch(line_segment.pill_collided_segment){
            case PillCollidedSegment::TOP_CIRCLE:{
                _velocity = DynamicCircleVsRay(top_circle, velocity_delta_time, line_segment.line_segment);
            }
            break;
            case PillCollidedSegment::BOTTOM_CIRCLE:{
                _velocity = DynamicCircleVsRay(bottom_circle, velocity_delta_time, line_segment.line_segment);
            
                _pill->collision_body->collision_normal += line_segment.line_segment.Normal().norm();
                _pill->collision_body->collision_normal.norm();
            }
            break;
            case PillCollidedSegment::LEFT_SIDE:{
                _velocity.x -= (velocity_delta_time.x * (1.0f-line_segment.collision_time)-0.001f);
            }
            break;
            case PillCollidedSegment::RIGHT_SIDE:{
                _velocity.x -= (velocity_delta_time.x * (1.0f-line_segment.collision_time)+0.001f);
            }
            break;
        }
        ResolvePillVsLine(_pill, _velocity);    
    }
    

}

void
CollisionSystem::ResolveCircleVsLine202412(CollisionCircle* _collision_shape, CollisionBody* _collision_body, olc::vf2d& _velocity, std::vector<std::pair<float, Ray2>>& _collision_history){
    if(_velocity.mag() == 0.0f) return;
    collision_search_depth++;
    //if(collision_search_depth > 0) Console::Out(collision_search_depth);
    if(collision_search_depth > 50){
        //Console::Out("maximum recursion depth was reached");
        return;
    }
    
    std::vector<std::pair<float, Ray2>> hit_line_segments;

    for(int line_index = 0; line_index < lines.size(); line_index++){

        //Make line out of circle
        Shape<Circle> dynamic_circle = Shape<Circle>(Circle(olc::vf2d(0.0f, 0.0f),_collision_shape->radius), lines[line_index].Start());
    
        dynamic_circle.position = dynamic_circle.local_position;

        RayVsCircleCollisionData result = ufoMaths::RayVsCircle(Ray2(_collision_shape->position, _collision_shape->position + _velocity), &dynamic_circle);

        //Console::Out(dynamic_circle.position, dynamic_circle.local_position);

        if(result.is_hit){

            olc::vf2d line_size = Ray2(olc::vf2d(0.0f, 0.0f),result.collision_normal).Normal()*-5.0f;

            Ray2 corner_line = Ray2(lines[line_index].Start() - line_size, lines[line_index].Start() + line_size);

            //Getting the new line for drawing purposes.
            Ray2 new_ray = GetRayOffsetByCircleRadius(*_collision_shape, _velocity, corner_line);
            modified_lines.push_back(new_ray);
            hit_line_segments.push_back(std::make_pair(result.collision_time,corner_line));

        }
    }

    for(const auto& line : lines){
        Ray2 new_ray = GetRayOffsetByCircleRadius(*_collision_shape, _velocity, line);
        modified_lines.push_back(new_ray);
        CollisionData collision_data = ufoMaths::RayVsRay(Ray2(_collision_shape->position, _collision_shape->position+_velocity), new_ray);

        if(collision_data.is_hit){
            hit_line_segments.push_back(std::make_pair(collision_data.collision_time,line));
        }
    }

    //If a is less than b, swap.
    std::sort(hit_line_segments.begin(), hit_line_segments.end(),[&](const auto _a, const auto _b){return _b.first > _a.first;});

    if(hit_line_segments.size() > 0) _collision_shape->collision_happened_this_frame = true;

    if(hit_line_segments.size() > 0){

        std::pair<float, Ray2> line = hit_line_segments[0];

        //Would be good if this function could be some sort of black box that calculates the new velocity for respective shape.
        _velocity = DynamicCircleVsRay(*_collision_shape, _velocity, line.second);

        if(_collision_history.size() > 1){
            for(const auto& history_line : _collision_history){
                if(line.second.Start() == line.second.Start() && line.second.End() == line.second.End())
                {
                    
                    Console::Out("Collided into the same line twice");

                    if(ufoMaths::DotProduct(_collision_history[0].second.Normal().norm(), _collision_history[1].second.Normal().norm()) < 0.0f){
                        CollisionData data = ufoMaths::RayVsRay(GetRayOffsetByCircleRadius(*_collision_shape, _velocity, _collision_history[0].second), GetRayOffsetByCircleRadius(*_collision_shape, _velocity, _collision_history[1].second));
                        Console::Out("Cornered.");
                        if(data.is_hit){
                            Console::Out("is_hit.");
                            _velocity = {0.0f, 0.0f};
                            _collision_body->collision_normal = _collision_history[0].second.Normal().norm()+_collision_history[1].second.Normal().norm();
                            Console::Out(data.intersection_point);
                            Console::Out(Engine::Get().current_level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()));
                        }
                    }

                    return;
                        
                }
            }
        }

        _collision_shape->collision_body->collision_normal += line.second.Normal().norm();

        _collision_history.push_back(hit_line_segments[0]);

        ResolveCircleVsLine202412(_collision_shape, _collision_body, _velocity, _collision_history);
    }

}

void CollisionSystem::Draw(Camera* _camera){
    if(!Engine::Get().all_shapes_visible) return;
    for(auto line : lines){
        DrawingSystem::Draw(line, _camera, olc::MAGENTA);
        olc::vf2d middle_position = line.Start()+(line.End()-line.Start())/2.0f;
        DrawingSystem::Draw(Ray2(middle_position, middle_position+line.Normal().norm()*5.0f), _camera, olc::MAGENTA);
    }
    if(!Engine::Get().modified_collision_lines_visible) return;
    for(auto line : modified_lines){
        DrawingSystem::Draw(line, _camera, olc::Pixel(255, 0, 0, 40));
        olc::vf2d middle_position = line.Start()+(line.End()-line.Start())/2.0f;
        DrawingSystem::Draw(Ray2(middle_position, middle_position+line.Normal().norm()*5.0f), _camera, olc::Pixel(255, 0, 0, 40));
    }
    
    modified_lines.clear();
}