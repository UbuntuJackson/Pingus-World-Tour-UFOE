#pragma once

#include "../bitmap_level/bitmap_collision_body.h"
#include "../level/level.h"
#include "../ufo_maths/ufo_maths.h"
#include "../animation/animation.h"
#include "../animation/animated_sprite_reference.h"
#include "../keyboard/single_keyboard.h"
#include "../console/console.h"
#include "pingus_level.h"
#include "../ufo_maths/ufo_maths.h"
#include "../shapes/rectangle.h"
#include "../mouse/mouse.h"

class Pingu : public BitmapCollisionBody{
public:
    Animation* anim = nullptr;
    float face_direction = 1.0f;
    bool hit_floor_last_frame = false;
    PingusLevel* level;

    float max_slope_height = 3.0f;

    Pingu(olc::vf2d _local_position, Level* _level) : BitmapCollisionBody(_local_position, _level, "box_12_24", "solid"){
        
    }
    void OnLevelEnter(Level* _level){
        BitmapCollisionBody::OnLevelEnter(_level);
        anim = AddChild<Animation>(Vector2f(8.0f, 10.0f));
        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_walk",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,16.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            20.0f
        ));
        anim->SetAnimation("pingu_walk");

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_explode",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,16.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        level = dynamic_cast<PingusLevel*>(_level);
        
    }
    void OnUpdate(){

        //Console::Out(anim->current_animation_state);

        if(anim->current_animation_state->key == "pingu_explode"){
            //Console::Out("Frame index",anim->current_animation_state->frame_counter);
            if(anim->current_animation_state->current_frame_index > 4.9){
                for(auto&& [k,v] : level->level_decals){
                    if(k == "bg") continue;
                    
                    for(int yy = -32+12; yy < 32+12; yy++){
                        for(int xx = -32+6; xx < 32+6; xx++){
                            if(ufoMaths::Distance2(local_position+Vector2f(xx,yy), local_position+Vector2f(6.0f,12.0f)) < 32.0f){
                                v->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(0,0,0,0));
                            }
                        }
                    }
                    v->Update();
                }
            
            }
            if(anim->cycle_count > 0){
                QueueForPurge();
            }
            
            return;
        }
        
        
        if(RectangleVsPoint(Rectangle(local_position, Vector2f(12.0f,24.0f)),level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition())) && Mouse::Get().GetRightButton().is_pressed){
            anim->SetAnimation("pingu_explode");
            anim->frame_counter = 0.0f;
            
            Console::Out("Frame index",anim->current_animation_state->current_frame_index);
            return;
        }

        //BitmapCollisionBody::OnUpdate();

        //return;
        anim->current_animation_state->scale.x = face_direction;
        velocity.x = face_direction * 30.0f;
        velocity.y += 60.0f * Engine::Get().GetDeltaTime();
        
        PinguCollision();

        /*ApplyCollision(game);

        local_position = position;*/

        /*if(hit_slope || hit_wall || hit_ceiling || hit_floor){
            Console::Out("Hit slope",hit_slope);
            Console::Out("Hit wall",hit_wall);
            Console::Out("Hit ceiling",hit_ceiling);
            Console::Out("Hit floor",hit_floor);
            Console::Out("---");
        }*/

        if(hit_wall) face_direction *= -1.0f;

    }

    void PinguCollision(){
        hit_floor_last_frame = hit_floor;

        hit_wall = false;
        hit_slope = true;
        hit_floor = false;
        hit_ceiling = false;

        Vector2f position_before_slope_detected = local_position;

        local_position.x += velocity.x * Engine::Get().GetDeltaTime();

        //Normal slope and walls
        if(IsOverlapping(game, mask_decal,solid_layer,local_position)){
            bool slope_resolved = false;
            
            while(!slope_resolved){

                position_before_slope_detected.x += ufoMaths::Sign(velocity.x);

                Vector2f position_before_slope_check = position_before_slope_detected;
                while(IsOverlapping(game, mask_decal,solid_layer,position_before_slope_detected)){
                    position_before_slope_detected.y-=1.0f;

                    if(std::abs(position_before_slope_detected.y - position_before_slope_check.y) > max_slope_height){
                        hit_slope = false;
                        hit_wall = true;

                        position_before_slope_detected.x -= ufoMaths::Sign(velocity.x);
                        velocity.x = 0.0f;

                        local_position.x = position_before_slope_detected.x;
                        slope_resolved = true;
                        hit_wall = true;
                        break;
                        
                    }
                }

                if(std::abs(local_position.x - position_before_slope_detected.x) > std::abs(velocity.x * Engine::Get().GetDeltaTime())){
                    slope_resolved = true;
                }
            }
            if(hit_slope) local_position.y = position_before_slope_detected.y;
            
        }
        else{
            hit_slope = false;
        }

        //Semi solid slope

        bool hit_semisolid_slope = false;
        
        for(int i = 0; i < 12; i++){
            bool slope_resolved = false;
            if(game->level_decals[solid_layer]->sprite->GetPixel(local_position.x+(float)i,local_position.y + 23.0f) == olc::RED){
                hit_semisolid_slope = true;
                while(!slope_resolved){

                    position_before_slope_detected.x += ufoMaths::Sign(velocity.x);

                    Vector2f position_before_slope_check = position_before_slope_detected;
                    while(game->level_decals[solid_layer]->sprite->GetPixel(local_position.x+(float)i,position_before_slope_detected.y + 23.0f) == olc::RED){
                        position_before_slope_detected.y-=1.0f;
                    }

                    slope_resolved = true;
                }
                local_position.y = position_before_slope_detected.y;
                velocity.y = 0.0f;
                break;
            }

            if(game->level_decals[solid_layer]->sprite->GetPixel(local_position.x+(float)i,local_position.y + 23.0f) == olc::YELLOW){

                while(!slope_resolved){

                    position_before_slope_detected.x += ufoMaths::Sign(velocity.x);

                    Vector2f position_before_slope_check = position_before_slope_detected;
                    while(game->level_decals[solid_layer]->sprite->GetPixel(local_position.x+(float)i,position_before_slope_detected.y + 23.0f) == olc::YELLOW){
                        position_before_slope_detected.y-=1.0f;
                    }

                    slope_resolved = true;
                }
                local_position.y = position_before_slope_detected.y;
                velocity.y = 0.0f;
                break;
            }
        }
        

        local_position.y += velocity.y * Engine::Get().GetDeltaTime();

        if(IsOverlapping(game, mask_decal,solid_layer,local_position)){
            while(IsOverlapping(game, mask_decal,solid_layer,local_position)){
                local_position.y-=ufoMaths::Sign(velocity.y);
            }
            if(velocity.y > 0.0f) hit_floor = true;
            if(velocity.y < 0.0f) hit_ceiling = true;
            velocity.y = 0.0f;
        }

        if(IsOverlapping(game, mask_decal,solid_layer,local_position+Vector2f(0.0f, 1.0f))){
            hit_floor = true;
        }

        if(!hit_floor && hit_floor_last_frame && !hit_slope){

            bool found_slope = true;
            Vector2f temporary_position = local_position;

            while(!IsOverlapping(game, mask_decal, solid_layer, temporary_position)){
                temporary_position.y += 1.0f;
                if(std::abs(temporary_position.y - local_position.y) > max_slope_height*2.0f){
                    
                    found_slope = false;
                    break;
                }
            }
            if(found_slope){
                local_position.y = temporary_position.y-1.0f;
                //velocity.y = 100.0f;
            }
            
        }

        return;

        if(!(hit_semisolid_slope || hit_floor)){
            bool found_slope = true;
            
            Console::Out("Trying for semisolid snap to ground");
            for(int i = 0; i < 12; i++){
                Vector2f temporary_position = local_position;
                while(!((game->level_decals[solid_layer]->sprite->GetPixel(temporary_position.x+(float)i,temporary_position.y + 23.0f) == olc::RED)))
                {
                    temporary_position.y += 1.0f;
                    if(std::abs(temporary_position.y - local_position.y) > max_slope_height*2.0f){
                        
                        found_slope = false;
                        break;
                    }
                }
                
                if(found_slope){
                    local_position.y = temporary_position.y-1.0f;
                    //velocity.y = 100.0f;
                    break;
                }
                else{
                    Console::Out("Did not fine slope");
                }
            }
        }

        /*if(hit_floor){
            local_position.y = std::floor(local_position.y)+0.01;
        }*/
        
    }

    void OnDraw(Camera* _camera){
        //DrawingSystem::DrawString({0.0f, 0.0f}, "Hello world", olc::WHITE, {1.0f,1.0f});
    }

};