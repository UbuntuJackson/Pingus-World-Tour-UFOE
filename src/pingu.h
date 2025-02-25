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
#include <widget_sprite_reference.h>
#include "goal.h"
#include "honey_coin.h"
#include "item_select_menu.h"

class Pingu : public BitmapCollisionBody{
public:
    Animation* anim = nullptr;
    float face_direction = 1.0f;
    bool hit_floor_last_frame = false;
    bool snap_to_ground_enabled = true;
    PingusLevel* level;

    Timer build_timer;
    Timer fall_timer;

    bool is_in_special_state = false;
    bool exploded = false;
    int steps = 0;
    Vector2f build_location;

    enum States{
        WALK,
        FALL,
        BUILD,
        EXPLODE,
        DIE_BY_FALL,
        PARACHUTE,
        BLOCKER
    };

    int what_is_current_state = States::WALK;

    std::function<void()> state_walk = [this](){
        what_is_current_state = States::WALK;
        Walk();
    };

    std::function<void()> state_fall = [this](){
        what_is_current_state = States::FALL;
        Fall();
    };

    std::function<void()> state_build = [this](){
        what_is_current_state = States::BUILD;
        Build();
    };

    std::function<void()> state_explode = [this](){
        what_is_current_state = States::EXPLODE;
        Explode();
    };

    std::function<void()> die_by_fall = [this](){
        what_is_current_state = States::DIE_BY_FALL;
        DieByFall();
    };

    int parachute_sprite_id = -1;

    std::function<void()> state_parachute = [this](){
        what_is_current_state = States::PARACHUTE;
        Parachute();
    };

    std::function<void()> state_blocker = [this](){
        what_is_current_state = States::BLOCKER;
        Blocker();
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
            "pingu_walk_parachute",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,16.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            20.0f
        ));

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
            "pingu_fall_death",
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
            "pingu_blocker",
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

    bool has_parachute = false;

    void Parachute(){
        fall_timer.Start(5000000.0f);
        fall_timer.Stop();
        anim->SetAnimation("pingu_parachute");
        velocity.y = 20.0f;

        if(hit_floor){
            has_parachute = false;
            is_in_special_state = false;
            level->QueueForPurge(parachute_sprite_id);
            parachute_sprite_id = -1;
        }
    }

    void DieByFall(){
        velocity.x = 0.0f;
        if(anim->current_animation_state->key != "pingu_fall_death") anim->SetAnimation("pingu_fall_death");
        if(anim->cycle_count > 0) QueueForPurge();
    }

    void Walk(){
        anim->current_animation_state->scale.x = face_direction;
        velocity.x = face_direction * 30.0f;
        
        if(!has_parachute) anim->SetAnimation("pingu_walk");
        else anim->SetAnimation("pingu_walk_parachute");
        
        if(fall_timer.GetTimeLeft() <= 0.0f){
            if(IsOverlapping(level,mask_decal,solid_layer,local_position+Vector2f(0.0f,2.0f),olc::MAGENTA)) item_blow_up();
            else{
                state = die_by_fall;
            }

            is_in_special_state = true;
            
        }

        fall_timer.Start(5000000.0f);
        fall_timer.Stop();
    }

    void Fall(){
        if(!fall_timer.is_started) fall_timer.Start(3000.0f);
        anim->SetAnimation("pingu_fall");
        velocity.x = 0.0f;
        velocity.y = 100.0f;
        if(has_parachute){
            state = state_parachute;

            parachute_sprite_id = AddChild<SpriteReference>("parachute",
                Vector2f(-22.0f, -48.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(59.0f, 66.0f),
                Vector2f(1.0f, 1.0f),
                0.0f
            )->GetID();

            is_in_special_state = true;
        }
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
            
            build_timer.Start(200.0f);
            for(int yy = 0; yy < 2; yy++){
                for(int xx = 6; xx < 6+(int)step_width; xx++){
                    
                    auto dec = level->level_decals.at("solid");
                    dec->sprite->SetPixel(build_location+Vector2f(xx+step_width*steps*face_direction,yy+24.0f-2.0f-steps*2),olc::Pixel(255,0,0,255));
                    dec->Update();

                    if(level->level_decals.count("mg")){
                        auto dec_mg = level->level_decals.at("mg");
                        dec_mg->sprite->SetPixel(build_location+Vector2f(xx+step_width*steps*face_direction,yy+24.0f-2.0f-steps*2),olc::Pixel(200,50,50,255));
                        dec_mg->Update();
                    }
                    
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
                            if(ufoMaths::Distance2(local_position+Vector2f(xx,yy), local_position+Vector2f(6.0f,12.0f)) <= 33.0f){
                                if(ufoMaths::Distance2(local_position+Vector2f(xx,yy), local_position+Vector2f(6.0f,12.0f)) > 28.0f
                                    && v->sprite->GetPixel(local_position+Vector2f(xx,yy)).a != 0
                                ){
                                    olc::Pixel c = v->sprite->GetPixel(local_position+Vector2f(xx,yy));
                                    v->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(25,25,25));
                                }
                                else v->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(0,0,0,0));
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

    bool is_already_blocker = false;

    void Blocker(){
        velocity.x = 0.0f;

        int width = 2;

        if(
            RectangleVsPoint(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()))
            && Mouse::Get().GetLeftButton().is_pressed && is_already_blocker
        )
        {
            Console::Out("removed stuff");
            for(int yy = 0; yy < 6; yy++){
                for(int xx = 5; xx < 5+width; xx++){
                    
                    auto dec = level->level_decals.at("solid");

                    Vector2f place_pos = local_position+Vector2f(xx,24.0f-yy);
                    if(dec->sprite->GetPixel(place_pos) == olc::BLUE) dec->sprite->SetPixel(place_pos,olc::Pixel(0,0,0,0));
                    dec->Update();
                    Console::Out("made blue",xx,yy);
                    
                    //level->level_decals.at("mg")->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(255,0,0,255));
                    
                }
            }

            state = state_walk;
            is_already_blocker = false;
            is_in_special_state = false;
            return;
        }
        is_already_blocker = true;
    }

    std::function<bool()> item_block = [this](){
        if(
            what_is_current_state == States::BLOCKER ||
            what_is_current_state == States::FALL ||
            what_is_current_state == States::PARACHUTE
        ) return false;
        anim->SetAnimation("pingu_blocker");
        anim->frame_counter = 0.0f;
        //anim->current_animation_state->visible = false;
        
        int width = 2;
        Console::Out("Item block");

        auto dec = level->level_decals.at("solid");

        for(int yy = 0; yy < 6; yy++){
            for(int xx = 5; xx < 5+width; xx++){

                Vector2f place_pos = local_position+Vector2f(xx,24.0f-yy);
                
                olc::Pixel p = dec->sprite->GetPixel(place_pos);
                
                //This is not {0,0,0,0} for some reason but instead {115,121,121,0}
                Console::Out(
                    int(p.r),
                    int(p.g),
                    int(p.b),
                    int(p.a));
                
                if(dec->sprite->GetPixel(place_pos).a == 0){
                    //Console::Out("made blue",xx,yy);
                    dec->sprite->SetPixel(place_pos,olc::BLUE);
                }
                
                //level->level_decals.at("mg")->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(255,0,0,255));
                
            }
        }

        dec->Update();

        state = state_blocker;

        is_in_special_state = true;
        return true;
    };

    std::function<bool()> item_blow_up = [this](){
        if(what_is_current_state == States::EXPLODE) return false;
        anim->SetAnimation("pingu_explode");
        anim->frame_counter = 0.0f;
        
        state = state_explode;

        is_in_special_state = true;
        return true;
    };

    std::function<bool()> item_build = [this](){
        if(!hit_floor || what_is_current_state == States::BUILD || what_is_current_state == States::EXPLODE) return false;

        anim->SetAnimation("pingu_walk");

        build_location = local_position;
        
        state = state_build;
        build_timer.Start(200.0f);

        is_in_special_state = true;

        return true;
    };

    std::function<bool()> item_parachute = [this](){
        
        if(has_parachute) return false;

        has_parachute = true;

        return true;
    };

    std::vector<std::function<bool()>> items = {
        item_blow_up,
        item_build,
        item_parachute
    };

    int current_item = 2;

    void OnUpdate(){

        bool should_set_pingu_selected = false;
        level->at_least_one_pingu_active = true;

        if(!level->pingu_selected_this_frame && RectangleVsPoint(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()))){
            anim->current_animation_state->tint = olc::GREEN;
            should_set_pingu_selected = true;
        }
        else{
            anim->current_animation_state->tint = olc::WHITE;
        }
        
        if(!level->pingu_selected_this_frame && RectangleVsPoint(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition())) && Mouse::Get().GetLeftButton().is_pressed){

            if(level->item_select_menu != nullptr) level->item_select_menu->items[level->item_select_menu->selected_index](this);

        }

        if(should_set_pingu_selected) level->pingu_selected_this_frame = true;

        if(!is_in_special_state){
            if(hit_floor){
                state = state_walk;
                for(const auto& goal : level->goals){
                    if(ufoMaths::RectangleVsRectangle(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),goal->shape)){
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
            if(ufoMaths::RectangleVsCircle(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),honey_coin->shape)){
                level->honey_coin_hud->current_frame_index = 0.0f;
                honey_coin->QueueForPurge();
            }
        }

        state();
        
        bool is_already_overlapping_blue = IsOverlappingFeet(local_position,olc::BLUE);

        PinguCollision();

        /*if(hit_slope || hit_wall || hit_ceiling || hit_floor){
            Console::Out("Hit slope",hit_slope);
            Console::Out("Hit wall",hit_wall);
            Console::Out("Hit ceiling",hit_ceiling);
            Console::Out("Hit floor",hit_floor);
            Console::Out("---");
        }*/
        /*for(int i = 0; i++; i < 3){
            if(IsOverlappingFeet(local_position-Vector2f(0.0f,i),olc::BLUE) && !is_already_overlapping_blue){
                face_direction *= -1.0f;
            }
        }*/

        if(is_already_overlapping_blue) Console::Out("is_already_overlapping_blue");

        if(IsOverlappingFeet(local_position,olc::BLUE) && !is_already_overlapping_blue){
            face_direction *= -1.0f;
        }

        if(hit_wall) face_direction *= -1.0f;

    }

    void PinguCollision(){
        hit_floor_last_frame = hit_floor;

        hit_wall = false;
        hit_slope = true;
        hit_floor = false;
        hit_ceiling = false;

        bool attempt_free_from_semisolid = false;

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

        //Console::Out("");

        //Normal slope and walls
        if(IsOverlapping(game, mask_decal,solid_layer,local_position)){
            bool slope_resolved = false;
            Vector2f incrementing_position = local_position;

            while(IsOverlapping(game, mask_decal,solid_layer,incrementing_position)){
                incrementing_position.x -= ufoMaths::Sign(velocity.x);
            }
            
            while(!slope_resolved){

                Vector2f position_before_slope_incrementation = incrementing_position;

                while(IsOverlapping(game, mask_decal,solid_layer,incrementing_position)){
                    
                    incrementing_position.y-=1.0f;

                    if(std::abs(incrementing_position.y - position_before_slope_incrementation.y) > max_slope_height){
                        
                        hit_slope = false;
                        hit_wall = true;
                        incrementing_position = position_before_slope_incrementation;
                        incrementing_position.x -= ufoMaths::Sign(velocity.x);
                        slope_resolved = true;
                        break;
                        
                    }
                }

                if(!hit_wall) incrementing_position.x += ufoMaths::Sign(velocity.x);

                Console::Out("Less than max slope height", std::abs(incrementing_position.y - position_before_slope_incrementation.y));

                if(std::abs(local_position.x - incrementing_position.x) >= std::abs(velocity.x * Engine::Get().GetDeltaTime())){
                    slope_resolved = true;
                    Console::Out("Slope still not resolved", velocity.x * Engine::Get().GetDeltaTime());
                }
            }
            if(hit_slope){
                local_position.y = incrementing_position.y;
                Console::Out("Hit slope confirmed", std::abs(velocity.x * Engine::Get().GetDeltaTime()));
            }
            if(hit_wall){
                Console::Out("Hit wall confirmed", std::abs(velocity.x * Engine::Get().GetDeltaTime()));
                local_position = incrementing_position;
                velocity.x = 0.0f;
            }
            
        }
        else{
            hit_slope = false;
        }

        if(IsOverlapping(game, mask_decal,solid_layer,local_position)) Console::Out("Is still overlapping after resolution");

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