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
#include <colour_utils.h>
#include "item_select_menu.h"

/// @spawn;
class Pingu : public BitmapCollisionBody{
public:
    bool is_anti_matter = false;

    Animation* anim = nullptr;
    float face_direction = 1.0f;
    bool hit_floor_last_frame = false;
    bool snap_to_ground_enabled = true;
    PingusLevel* level;

    Timer build_timer;
    Timer fall_timer;
    Timer driller_timer;

    bool is_in_special_state = false;
    bool exploded = false;
    int steps = 0;
    Vector2f build_location;

    bool is_rescued = false;

    enum States{
        WALK,
        FALL,
        BUILD,
        EXPLODE,
        DIE_BY_FALL,
        PARACHUTE,
        BLOCKER,
        CLIMBER,
        FALL_AFTER_CLIMBER,
        DRILLER
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

    std::function<void()> state_climber = [this](){
        what_is_current_state = States::CLIMBER;
        Climber();
    };

    std::function<void()> state_fall_after_climber = [this](){
        what_is_current_state = States::FALL_AFTER_CLIMBER;
        FallAfterClimber();
    };

    std::function<void()> state_driller = [this](){
        Driller();
    };

    std::function<void()> state = state_walk;

    float max_slope_height = 3.0f;

    Pingu(olc::vf2d _local_position) : BitmapCollisionBody(_local_position, "box_12_24", "solid"){
        
    }
    void OnLevelEnter(Level* _level){
        
        BitmapCollisionBody::OnLevelEnter(_level);

        anim = AddChild<Animation>(Vector2f(6.0f, 19.0f));
        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_walk",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
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
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            20.0f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_explode",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_fall_death",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_fall",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_blocker",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_parachute",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            10.6f
        ));

        anim->AddAnimationState(AnimatedSpriteReference(
            "pingu_driller",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f,24.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f,
            0,
            20.0f
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

    void OnStart(Level* _level){
        if(is_anti_matter){
            level->anti_matter_pingus.push_back(this);
        }
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
        if(anim->cycle_count > 0){
            QueueForPurge();
        }
    }

    void Walk(){
        anim->current_animation_state->scale.x = face_direction;
        velocity.x = face_direction * 30.0f;

        if(has_climber && hit_wall){
            
            state = state_climber;
            is_in_special_state = true;
            
        }
        
        if(has_climber) anim->current_animation_state->scale.x = climbing_direction;

        if(!has_parachute) anim->SetAnimation("pingu_walk");
        else anim->SetAnimation("pingu_walk_parachute");
        
        if(fall_timer.GetTimeLeft() <= 0.0f){
            if(IsOverlapping(level,mask_decal,solid_layer,local_position+Vector2f(0.0f,2.0f),olc::MAGENTA)){
                
                for(auto&& [k,v] : level->level_decals){
                    
                    if(k == "bg") continue;
                    
                    for(int yy = -32+12; yy < 32+12; yy++){
                        for(int xx = -32+6; xx < 32+6; xx++){
                            if(ufoMaths::Distance2(local_position+Vector2f(xx,yy), local_position+Vector2f(6.0f,12.0f)) <= 32.0f){
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
                
            }
            
            state = die_by_fall;

            is_in_special_state = true;
            
        }

        //To make sure the timer does not reach below zero when entering fall state again.
        fall_timer.Start(5000000.0f);
        fall_timer.Stop();
    }

    void Fall(){
        if(!fall_timer.is_started) fall_timer.Start(1975.0f);
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
        float step_width = 3.0f;
        int number_of_steps = 25;

        snap_to_ground_enabled = false;

        if(steps == number_of_steps || hit_wall || hit_slope || velocity.x == 0.0f || IsOverlappingHead(local_position,olc::WHITE)){
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
                for(int xx = 6; xx < 6+(int)step_width*2; xx++){
                    
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
            
            if(anim->current_animation_state->current_frame_index > 4.9 && !exploded){
                    
                for(int yy = -32+12; yy < 32+12; yy++){
                    for(int xx = -32+6; xx < 32+6; xx++){
                        for(auto&& [k,v] : level->level_decals){
                            if(k == "bg") continue;

                            auto solid_decal = level->level_decals.at(solid_layer);
                            if(CompareColour(solid_decal->sprite->GetPixel(local_position+Vector2f(xx,yy)), olc::VERY_DARK_GREY)) continue;

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
                }
                for(auto&& [k,v] : level->level_decals){
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
            
            for(int yy = 0; yy < 6; yy++){
                for(int xx = 5; xx < 5+width; xx++){
                    
                    auto dec = level->level_decals.at("solid");

                    Vector2f place_pos = local_position+Vector2f(xx,24.0f-yy);
                    if(dec->sprite->GetPixel(place_pos) == olc::BLUE) dec->sprite->SetPixel(place_pos,olc::Pixel(0,0,0,0));
                    dec->Update();
                    
                    
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

    float climbing_direction = 1.0f;

    void Climber(){

        velocity.y = -50.0f;

        anim->current_animation_state->scale.x = climbing_direction;
        anim->current_animation_state->rotation = climbing_direction*90.0f * 180.0f/3.1415f;

        snap_to_ground_enabled = false;

        if(
            hit_ceiling ||
            (!IsOverlappingFeet(local_position + Vector2f(climbing_direction * 1.0f, 0.0f), olc::WHITE))
            //!IsOverlappingSolid(local_position + Vector2f(climbing_direction * 1.0f, 0.0f))
        ){
            anim->current_animation_state->rotation = 0.0f;
            if(!hit_ceiling){
                Console::PrintLine("Climber hit wall", hit_wall);
                state = state_fall_after_climber;
            }
            else{
                Console::PrintLine("Climber hit ceiling.");
                climbing_direction *= -1.0f;
                is_in_special_state = false;
                state = state_walk;
                snap_to_ground_enabled = true;
            }
            velocity.y = 0.0f;
            
            face_direction = climbing_direction;
        }
    }

    void FallAfterClimber(){
        
        velocity.x = face_direction * 30.0f;
        
        Console::Print("FallAfterClimber\n");

        if(hit_wall){
            climbing_direction *= -1.0f;
            face_direction = climbing_direction;
            state = state_fall;
            is_in_special_state = false;
            snap_to_ground_enabled = true;
            return;
        }

        if(hit_floor){
            state = state_walk;
            is_in_special_state = false;
            snap_to_ground_enabled = true;
        }
    }

    void Driller(){
        velocity.x = 0.0f;
        velocity.y = 10.0f;
        anim->SetAnimation("pingu_driller");
        if(driller_timer.GetTimeLeft() < 0.0f){
            driller_timer.Start(driller_pace);
            
            for(int yy = -32+12; yy < 32+12; yy++){
                for(int xx = -32+6; xx < 32+6; xx++){
                    auto solid_decal = level->level_decals.at(solid_layer);
                    if(CompareColour(solid_decal->sprite->GetPixel(local_position+Vector2f(xx,yy)), olc::VERY_DARK_GREY)) continue;

                    for(auto&& [k,v] : level->level_decals){
                        if(k == "bg") continue;
                        if(ufoMaths::Distance2(local_position+Vector2f(xx,yy), local_position+Vector2f(6.0f,12.0f)) <= 14.0f){
                            if(ufoMaths::Distance2(local_position+Vector2f(xx,yy), local_position+Vector2f(6.0f,12.0f)) > 10.0f
                                && v->sprite->GetPixel(local_position+Vector2f(xx,yy)).a != 0
                            ){
                                olc::Pixel c = v->sprite->GetPixel(local_position+Vector2f(xx,yy));
                                v->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(25,25,25));
                            }
                            else v->sprite->SetPixel(local_position+Vector2f(xx,yy),olc::Pixel(0,0,0,0));
                        }
                    }
                    
                }
            }

            for(auto&& [k,v] : level->level_decals){
                v->Update();
            }
            
        }
        if(!hit_floor || IsOverlappingFeet(local_position+Vector2f(0.0f,1.0f),olc::VERY_DARK_GREY)){
            state = state_walk;
            is_in_special_state = false;
            snap_to_ground_enabled = true;
        }
    }

    bool has_climber = false;

    std::function<bool()> item_climber = [this](){
        if(
            what_is_current_state == States::BLOCKER ||
            what_is_current_state == States::FALL ||
            what_is_current_state == States::PARACHUTE ||
            what_is_current_state == States::CLIMBER ||
            has_climber
        ) return false;

        has_climber = true;
        climbing_direction = face_direction;

        return true;
    };

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

        auto dec = level->level_decals.at("solid");

        for(int yy = 0; yy < 6; yy++){
            for(int xx = 5; xx < 5+width; xx++){

                Vector2f place_pos = local_position+Vector2f(xx,24.0f-yy);
                
                olc::Pixel p = dec->sprite->GetPixel(place_pos);
                
                //This is not {0,0,0,0} for some reason but instead {115,121,121,0}
                
                if(dec->sprite->GetPixel(place_pos).a == 0){
                    dec->sprite->SetPixel(place_pos,olc::BLUE);
                }
                
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
        
        if(has_parachute || what_is_current_state == States::PARACHUTE) return false;

        has_parachute = true;

        return true;
    };

    float driller_pace = 50.0f;

    std::function<bool()> item_driller = [this](){
        if(!hit_floor && what_is_current_state == States::DRILLER) return false;
        is_in_special_state = true;
        state = state_driller;
        driller_timer.Start(driller_pace);
        //snap_to_ground_enabled = false;
        return true;
    };

    std::vector<std::function<bool()>> items = {
        item_blow_up,
        item_build,
        item_parachute
    };

    int current_item = 2;

    void PinguUpdate(){
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

        if(!is_anti_matter && what_is_current_state != States::EXPLODE){
            for(const auto& anti_matter_pingu : level->anti_matter_pingus){
                if(RectangleVsRectangle(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)), ufo::Rectangle(anti_matter_pingu->local_position, Vector2f(12.0f,24.0f)))){
                    item_blow_up();
                    anti_matter_pingu->item_blow_up();
                    anim->current_animation_state->current_frame_index = 4.9f;
                    anim->frame_counter = 4.9f;
                    anti_matter_pingu->anim->current_animation_state->current_frame_index = 4.9f;
                    anti_matter_pingu->anim->frame_counter = 4.9f;
                }
            }
        }

        if(!is_in_special_state){
            if(hit_floor){
                state = state_walk;
                for(const auto& goal : level->goals){
                    if(ufoMaths::RectangleVsRectangle(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),goal->shape)){
                        is_rescued = true;
                    }
                }
            }
            else{
                state = state_fall;
            }
        }

        if(local_position.y > level->level_size.y) QueueForPurge();

        for(const auto& honey_coin : level->honey_coin_handles){
            if(ufoMaths::RectangleVsCircle(ufo::Rectangle(local_position, Vector2f(12.0f,24.0f)),honey_coin->shape)){
                level->honey_coin_hud->current_frame_index = 0.0f;
                honey_coin->QueueForPurge();
            }
        }

        state();
        
        bool is_already_overlapping_blue = IsOverlappingFeet(local_position,olc::BLUE);

        //Attempt to create depth when pingus climb up semisolid and overlap pingu on lower level
        if(int(former_position.y) != int(local_position.y)) level->should_resort_after_z_index = true;

        former_position = local_position;

        PinguCollision();

        if(IsOverlappingFeet(local_position,olc::BLUE) && !is_already_overlapping_blue){
            face_direction *= -1.0f;
        }

        if(hit_wall && !has_climber){
            face_direction *= -1.0f;
            
        }
    }

    void OnPaused(){
        
        build_timer.Pause();
        fall_timer.Pause();
        
    }

    void OnUpdate(){
        build_timer.Resume();
        fall_timer.Resume();

        /*if(level->fast_forward){
            build_timer.FastForward(level->number_of_updates_when_fast_forward);
            fall_timer.FastForward(level->number_of_updates_when_fast_forward);
        }*/

        PinguUpdate();

        if(is_rescued){
            level->rescued_pingus++;
            QueueForPurge();
        }

        //Draft for way of speeding up pingus when holding space
        //This requires accounting for spawners though which have a fixed time interval between spawns.
        if(level->fast_forward){
            for(int _ = 0; _ < level->number_of_updates_when_fast_forward; _++)
            {
                build_timer.FastForward(1.0f);
                fall_timer.FastForward(1.0f);
                driller_timer.FastForward(1.0f);
                PinguUpdate();
            }
        }

        if(is_anti_matter){
            anim->current_animation_state->tint = olc::BLACK;
        }

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

        //Normal slope and walls
        if(IsOverlappingSolid(local_position)){
            bool slope_resolved = false;
            Vector2f incrementing_position = local_position;

            while(IsOverlappingSolid(incrementing_position)){
                incrementing_position.x -= ufoMaths::Sign(velocity.x);
            }
            
            while(!slope_resolved){

                Vector2f position_before_slope_incrementation = incrementing_position;

                while(IsOverlappingSolid(incrementing_position)){
                    
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

                if(std::abs(local_position.x - incrementing_position.x) >= std::abs(velocity.x * Engine::Get().GetDeltaTime())){
                    slope_resolved = true;
                    
                }
            }
            if(hit_slope){
                local_position.y = incrementing_position.y;
                
            }
            if(hit_wall){
                
                local_position = incrementing_position;
                velocity.x = 0.0f;
            }
            
        }
        else{
            hit_slope = false;
        }

        if(IsOverlappingSolid(local_position)) Console::Print("Is still overlapping after resolution\n");

        //Semi solid slope

        bool hit_semisolid_slope = false;
        
        if(IsOverlappingFeet(local_position, olc::RED)){
            hit_semisolid_slope = true;
            
            bool slope_resolved = false;

            Vector2f incrementing_position = local_position;

            Vector2f original_position_before_slope_check = local_position;

            while(IsOverlappingFeet(incrementing_position, olc::RED)){
                    
                incrementing_position.y-=1.0f;

                if(IsOverlappingSolid(incrementing_position)){
                    
                    incrementing_position.y+=1.0f;
                    
                    face_direction *= -1.0f;
                    break;
                }

            }
            
            local_position = incrementing_position;
            velocity.y = 0.0f;      
            
        }

        local_position.y += velocity.y * Engine::Get().GetDeltaTime();

        if(IsOverlappingSolid(local_position)){
            while(IsOverlappingSolid(local_position)){
                local_position.y-=ufoMaths::Sign(velocity.y);
            }
            if(velocity.y > 0.0f) hit_floor = true;
            if(velocity.y < 0.0f) hit_ceiling = true;
            velocity.y = 0.0f;
        }

        if(IsOverlappingSolid(local_position+Vector2f(0.0f, 1.0f)) || IsOverlappingFeet(local_position+Vector2f(0.0f, 1.0f), olc::RED)){
            hit_floor = true;
        }

        if(!hit_floor && hit_floor_last_frame && !hit_slope && snap_to_ground_enabled){

            bool found_slope = true;
            Vector2f temporary_position = local_position;

            while(!IsOverlappingSolid(temporary_position) && !IsOverlapping(game, mask_decal, solid_layer, temporary_position, olc::RED)){
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

    bool IsOverlappingSolid(Vector2f _check_location){
        return IsOverlapping(game, mask_decal, solid_layer, _check_location, olc::VERY_DARK_GREY) || IsOverlapping(game, mask_decal, solid_layer, _check_location, olc::WHITE);
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