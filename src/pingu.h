#pragma once

#include <bitmap_collision_body.h>
#include <level.h>
#include <ufo_maths.h>
#include <animation.h>
#include <animated_sprite_reference.h>
#include <single_keyboard.h>
#include <console.h>
#include <sprite_reference.h>
#include "pingus_level.h"
#include <ufo_maths.h>
#include <rectangle.h>
#include <mouse.h>
#include <timer.h>
#include "goal.h"
#include "honey_coin.h"

class Pingu : public BitmapCollisionBody{
public:
    Animation* anim = nullptr;
    float face_direction = 1.0f;
    bool hit_floor_last_frame = false;
    bool snap_to_ground_enabled = true;
    PingusLevel* level;

    Timer build_timer;

    bool is_in_special_state = false;
    bool exploded = false;
    int steps = 0;
    Vector2f build_location;

    std::function<void()> state_walk = [this](){
        Walk();
    };

    std::function<void()> state_fall = [this](){
        Fall();
    };

    std::function<void()> state_build = [this](){
        Build();
    };

    std::function<void()> state_explode = [this](){
        Explode();
    };

    int parachute_sprite_id = -1;

    std::function<void()> state_parachute = [this](){
        Parachute();
    };

    std::function<void()> state = state_walk;

    float max_slope_height = 3.0f;

    Pingu(olc::vf2d _local_position) : BitmapCollisionBody(_local_position, "box_12_24", "solid"){
        
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

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_fall",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,16.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_parachute",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,16.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        if(Engine::Get().all_shapes_visible){
            AddChild<SpriteReference>(
                mask,Vector2f(0.0f, 0.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(12.0f, 24.0f),
                Vector2f(1.0f, 1.0f),
                0.0f
            )->tint.a = 100;
        }

        level = dynamic_cast<PingusLevel*>(_level);
        
        level->released_pingus++;
        
    }

    void Parachute(){
        anim->SetAnimation("pingu_parachute");
        velocity.y = 20.0f;

        if(hit_floor){
            is_in_special_state = false;
            level->QueueForPurge(parachute_sprite_id);
            parachute_sprite_id = -1;
        }
    }

    void Walk(){
        anim->current_animation_state->scale.x = face_direction;
        velocity.x = face_direction * 30.0f;
        
        anim->SetAnimation("pingu_walk");
        
    }

    void Fall(){
        anim->SetAnimation("pingu_fall");
        velocity.x = 0.0f;
        velocity.y = 100.0f;
    }

    void Build(){
        float step_width = 5.0f;
        int number_of_steps = 18;

        snap_to_ground_enabled = false;

        if(steps == number_of_steps || hit_wall || velocity.x == 0.0f || IsOverlappingHead(local_position,olc::WHITE)){
            is_in_special_state = false;
            steps = 0;
            snap_to_ground_enabled = true;
            return;
        }

        bool reached_location = false;

        float destination_x = build_location.x + face_direction * step_width * steps;
        if(face_direction > 0.0f){
            if(local_position.x < destination_x) velocity.x = 6.0f;
            else reached_location = true;
        }

        if(face_direction < 0.0f){
            if(local_position.x > destination_x) velocity.x = -6.0f;
            else reached_location = true;
        }
        
        if(reached_location){
            Console::Out("Build");
            build_timer.Start(200.0f);
            for(int yy = 0; yy < 2; yy++){
                for(int xx = 6; xx < 6+(int)step_width; xx++){
                    
                    auto dec = level->level_decals.at("solid");
                    dec->sprite->SetPixel(build_location+Vector2f(xx+step_width*steps*face_direction,yy+24.0f-2.0f-steps*2),olc::Pixel(255,0,0,255));
                    dec->Update();
                    
                    //level->level_decals.at("mg")->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(255,0,0,255));
                    
                }
            }
            steps++;
            
        }
        
    }

    void Explode(){
        velocity.x = 0.0f;

        if(anim->current_animation_state->key == "pingu_explode"){
            //Console::Out("Frame index",anim->current_animation_state->frame_counter);
            if(anim->current_animation_state->current_frame_index > 4.9 && !exploded){
                for(auto&& [k,v] : level->level_decals){
                    Console::Out(k);
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
                exploded = true;
            
            }
            if(anim->cycle_count > 0){
                QueueForPurge();
            }
            
        }
    }

    std::function<void()> item_blow_up = [this](){
        anim->SetAnimation("pingu_explode");
        anim->frame_counter = 0.0f;
        
        state = state_explode;

        Console::Out("Frame index",anim->current_animation_state->current_frame_index);
        is_in_special_state = true;
    };

    std::function<void()> item_build = [this](){
        if(!hit_floor) return;

        anim->SetAnimation("pingu_walk");

        build_location = local_position;
        
        state = state_build;
        build_timer.Start(200.0f);

        is_in_special_state = true;
    };

    std::function<void()> item_parachute = [this](){
        if(hit_floor) return;

        state = state_parachute;

        parachute_sprite_id = AddChild<SpriteReference>("parachute",
            Vector2f(-22.0f, -48.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(59.0f, 66.0f),
            Vector2f(1.0f, 1.0f),
            0.0f
        )->GetID();

        is_in_special_state = true;
    };

    std::vector<std::function<void()>> items = {
        item_blow_up,
        item_build,
        item_parachute
    };

    int current_item = 1;

    void OnUpdate(){
        
        if(RectangleVsPoint(Rectangle(local_position, Vector2f(12.0f,24.0f)),level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition())) && Mouse::Get().GetLeftButton().is_pressed){

            items[current_item]();

        }

        if(!is_in_special_state){
            if(hit_floor){
                state = state_walk;
                for(const auto& goal : level->goals){
                    if(ufoMaths::RectangleVsRectangle(Rectangle(local_position, Vector2f(12.0f,24.0f)),goal->shape)){
                        level->rescued_pingus++;
                        QueueForPurge();
                    }
                }
            }
            else{
                state = state_fall;
            }
        }

        for(const auto& honey_coin : level->honey_coin_handles){
            if(ufoMaths::RectangleVsCircle(Rectangle(local_position, Vector2f(12.0f,24.0f)),honey_coin->shape)){
                level->honey_coin_hud->current_frame_index = 0.0f;
                honey_coin->QueueForPurge();
            }
        }

        state();
        
        PinguCollision();

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

        bool attempt_free_from_semisolid = false;

        Vector2f position_before_slope_detected = local_position;

        is_already_in_semi_solid = IsOverlappingFeet(local_position, olc::RED);

        local_position.x += velocity.x * Engine::Get().GetDeltaTime();

        /*if(IsOverlapping(game,mask_decal,solid_layer,local_position)){
            Vector2f position_before_ceiling_slide = local_position;
            bool slope_resolved = false;
            while(!slope_resolved){
                local_position.x += ufoMaths::Sign(velocity.x);

                while(IsOverlapping(game, mask_decal,solid_layer, local_position)){
                    local_position.y+=1.0f;

                    if(std::abs(position_before_ceiling_slide.y-local_position.y) > max_slope_height){
                        slope_resolved = true;
                        local_position = position_before_ceiling_slide;
                        break;
                    }
                }
                if(std::abs(local_position.x - position_before_ceiling_slide.x) > std::abs(velocity.x * Engine::Get().GetDeltaTime())){
                    Console::Out(std::abs(position_before_ceiling_slide.y-local_position.y));
                    slope_resolved = true;
                    attempt_free_from_semisolid = true;
                }
            }
        }*/

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
        
        if(IsOverlappingFeet(local_position, olc::RED)){
            hit_semisolid_slope = true;
            
            bool slope_resolved = false;

            Vector2f incrementing_position = local_position;

            Vector2f original_position_before_slope_check = local_position;

            while(IsOverlappingFeet(incrementing_position, olc::RED)){
                    
                incrementing_position.y-=1.0f;

                if(IsOverlapping(game, mask_decal, solid_layer, incrementing_position)){
                    
                    incrementing_position.y+=1.0f;
                    
                    face_direction *= -1.0f;
                    break;
                }

            }

            Console::Out(local_position.y - incrementing_position.y);
            
            local_position = incrementing_position;
            velocity.y = 0.0f;      
            
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

        if(IsOverlapping(game, mask_decal,solid_layer,local_position+Vector2f(0.0f, 1.0f)) || IsOverlappingFeet(local_position+Vector2f(0.0f, 1.0f), olc::RED)){
            hit_floor = true;
        }

        if(!hit_floor && hit_floor_last_frame && !hit_slope && snap_to_ground_enabled){

            bool found_slope = true;
            Vector2f temporary_position = local_position;

            while(!IsOverlapping(game, mask_decal, solid_layer, temporary_position) && !IsOverlapping(game, mask_decal, solid_layer, temporary_position, olc::RED)){
                temporary_position.y += 1.0f;
                if(std::abs(temporary_position.y - local_position.y) > max_slope_height*2.0f){
                    
                    found_slope = false;
                    break;
                }
            }
            if(found_slope){
                hit_floor = true;
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
        
    }

    void OnDraw(Camera* _camera){
        //Engine::Get().pixel_game_engine.DrawDecal(_camera->Transform(local_position),mask_decal);
        //DrawingSystem::DrawString({0.0f, 0.0f}, "Hello world", olc::WHITE, {1.0f,1.0f});
    }

    bool IsOverlappingFeet(Vector2f _position, olc::Pixel _colour){
        for(int i = 0; i < 12; i++){
            if(game->level_decals[solid_layer]->sprite->GetPixel(_position.x+(float)i,_position.y + 23.0f) == _colour){
                return true;
            }
        }
        return false;
    }

    bool IsOverlappingHead(Vector2f _position, olc::Pixel _colour){
        for(int i = 0; i < 12; i++){
            if(game->level_decals[solid_layer]->sprite->GetPixel(_position.x+(float)i,_position.y) == _colour){
                return true;
            }
        }
        return false;
    }

};