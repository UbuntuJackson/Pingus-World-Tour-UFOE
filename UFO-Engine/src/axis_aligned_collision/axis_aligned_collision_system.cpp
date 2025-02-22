#include "axis_aligned_collision_system.h"
#include "axis_aligned_collision_body.h"
#include "../ufo_maths/ufo_maths.h"
#include "../drawing_system/drawing_system.h"
#include "../json/json.h"

void AxisAlignedCollisionSystem::LoadTiles(Json& _json){

}

void
AxisAlignedCollisionSystem::Update(AxisAlignedCollisionBody* _collisionbody){
    //std::vector<Rectangle> active_tiles;

    _collisionbody->velocity += _collisionbody->acceleration /* * Engine::Get().GetDeltaTime() */;
    _collisionbody->shape.position.y += _collisionbody->velocity.y;

    int collision_check_range_x;
    int collision_check_range_y;
    int collision_tile_type = 0; //square
    
    collision_check_range_x = int(_collisionbody->velocity.x)/collision_tile_width+int(ufoMaths::Sign(_collisionbody->velocity.x));
    collision_check_range_y = int(_collisionbody->velocity.y)/collision_tile_height+int(ufoMaths::Sign(_collisionbody->velocity.y));
    for(int y = 0; y < collision_check_range_y; y++){
        for(int x = 0; x < collision_check_range_x; x++){
            olc::vf2d tile_position = {_collisionbody->shape.position.x + (float)x, _collisionbody->shape.position.y + (float)y};
            ufo::Rectangle tile = ufo::Rectangle(tile_position, olc::vf2d(collision_tile_width, collision_tile_height));
            active_tiles.push_back(tile);
        }
    }

    for(const auto& tile : active_tiles){
        bool hit_y = false;
        while(ufoMaths::RectangleVsRectangle(_collisionbody->shape, tile)){
            hit_y = true;
            float direction = ufoMaths::Sign(_collisionbody->velocity.y);
            _collisionbody->shape.position.y -= direction;
        }
        if(hit_y) _collisionbody->velocity.y = 0.0f;
    }

    _collisionbody->shape.position.x += _collisionbody->velocity.x;

    for(const auto& tile : active_tiles){
        bool hit_x = false;
        while(ufoMaths::RectangleVsRectangle(_collisionbody->shape, tile)){
            hit_x = true;
            float direction = ufoMaths::Sign(_collisionbody->velocity.x);
            _collisionbody->shape.position.x -= direction;
        }
        if(hit_x) _collisionbody->velocity.x = 0.0f;
    }

    _collisionbody->local_position = _collisionbody->shape.position;

}

void AxisAlignedCollisionSystem::Draw(Camera* _camera){
    for(const auto& tile : active_tiles){
        DrawingSystem::Draw(tile, _camera, olc::MAGENTA);
    }
    active_tiles.clear();
}