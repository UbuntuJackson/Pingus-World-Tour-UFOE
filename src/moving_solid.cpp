#include <cmath>
#include <actor.h>
#include <ufo_maths.h>
#include <functional>
#include <level_sprite_reference.h>
#include <ufo_maths.h>
#include <ufo_engine.h>
#include <mouse.h>
#include <asset_manager.h>
#include <console.h>
#include "pingus_level.h"
#include "pingu.h"
#include "moving_solid.h"

MovingSolid::MovingSolid(Vector2f _local_position) : Actor(_local_position){

}

void MovingSolid::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level);
    level->asset_manager.LoadDecal("../res/assets/moving_solid.png","moving_solid");
    spr = AddChild<LevelSpriteReference>(
        "moving_solid",
        Vector2f(0.0f,0.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(100.0f, 100.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );
    SetZIndex(0);
    level->moving_solids_handles.push_back(this);

    max_left = local_position.x;
    max_right = local_position.x + 100.0f;
    
}

void MovingSolid::OnStart(Level* _level){
    
    centre_position = local_position;

    if(is_instantiated_via_editor){

        //Making sprite
        ufo::Rectangle rect = AssetManager::Get().GetFrameFromSpriteSheet(GetEditorCategory(), GetEditorSlotID()-1, Vector2f(100.0f,100.0f));

        std::unique_ptr<olc::Sprite> generated_sprite = std::make_unique<olc::Sprite>(rect.size.x, rect.size.y);
        for(int yy = (int)rect.position.y; yy < (int)rect.position.y + (int)rect.size.y; yy++){
            for(int xx = (int)rect.position.x; xx < (int)rect.position.x + (int)rect.size.x; xx++){
                generated_sprite->SetPixel(Vector2i(xx-int(rect.position.x), yy-int(rect.position.y)),AssetManager::Get().GetDecal(GetEditorCategory())->sprite->GetPixel(xx,yy));
            }
        }

        std::string name_plus_id = GetEditorCategory()+"_"+std::to_string(GetEditorSlotID());
        spr->key = name_plus_id;
        _level->asset_manager.sprites[name_plus_id] = std::move(generated_sprite);
        _level->asset_manager.decals[name_plus_id] = std::make_unique<olc::Decal>(_level->asset_manager.sprites[name_plus_id].get());
        _level->asset_manager.decals[name_plus_id]->Update();
    }
}

void MovingSolid::OnUpdate(){

    Vector2f delta_mouse = Vector2f(0.0f,0.0f);

    switch(movement_mode){
        case MovementModes::LEFT_RIGHT : {
            if(local_position.x < max_left) velocity.x = 100.0f;
            else if(local_position.x > max_right) velocity.x = -100.0f;
        }
        break;
        case MovementModes::CIRCLE : {
            Vector2f new_position = centre_position + Vector2f(std::cos(angle) * radius_x, std::sin(angle) * radius_y);
            velocity = (new_position-local_position)/Engine::Get().GetDeltaTime();
            angle+=angle_increment * Engine::Get().GetDeltaTime();
        }
        break;
        case MovementModes::STATIC_MOUSE : {
            Vector2f new_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
            delta_mouse = new_position - local_position;
        }
        break;
        case MovementModes::CIRCLE_MOUSE : {
            Vector2f new_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition()) + Vector2f(std::cos(angle) * radius_x, std::sin(angle) * radius_y);
            velocity = (new_position-local_position)/Engine::Get().GetDeltaTime();
            angle+=angle_increment * Engine::Get().GetDeltaTime();
        }
        break;
    }

    //Resolving in X-axis
    //{
        float total_movement_x = delta_mouse.x + velocity.x * Engine::Get().GetDeltaTime();
        
        //Vector2f former_position = local_position;
        local_position.x += total_movement_x;
        
        for(auto&& pingu : level->pingu_handles_all_pingus){
            bool pingu_was_moved = false;
            Vector2f pingu_former_position = pingu->local_position;

            //If is standing on solid
            if(pingu->IsOverlappingMovingSolid(pingu->local_position + Vector2f(total_movement_x,0.0f) + Vector2f(0.0f,1.0f), olc::WHITE, this)
                && !pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)
            ){
                pingu->local_position.x += total_movement_x;

                //To avoid rounding errors the pingu is pushed one pixel further if still in solid.
                if(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)){
                    pingu->local_position.x -= ufoMaths::PreciseSignFloat(total_movement_x);
                }
                pingu_was_moved = true;
            }

            if(ufoMaths::PreciseSignFloat(total_movement_x) == 0.0f) continue;

            //Pushes pingu if NOT on the slope
            while(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)){
                pingu->local_position.x += ufoMaths::PreciseSignFloat(total_movement_x);
                Console::PrintLine("MovingSolid pingu->local_position.x", total_movement_x, ufoMaths::Sign(total_movement_x));
                pingu_was_moved = true;
            }

            //If the pingu collides with ordinary solids
            //This could be faulty. Resolving without pingu touching MovingSolid
            while(pingu->IsOverlapping(pingu->level, pingu->mask_decal, pingu->solid_layer, pingu->local_position, olc::WHITE)){
                pingu->local_position.x -= ufoMaths::PreciseSignFloat(total_movement_x);
                pingu_was_moved = true;
            }
            
            /*if(pingu->IsOverlappingSolid(pingu->local_position)){
                pingu->state = pingu->die_by_fall;
                pingu->is_in_special_state = true;
                Console::PrintLine("Pingu got squashed");
            }*/

            if(pingu_was_moved){
                pingu->OnMoved(this);
            }
        }
    //}

    //Resolving in Y-axis
    //{
        float total_movement_y = delta_mouse.y + velocity.y * Engine::Get().GetDeltaTime();

        //Vector2f former_position = local_position;

        local_position.y += total_movement_y;

        for(auto&& pingu : level->pingu_handles_all_pingus){
            bool pingu_was_moved = false;
            Vector2f pingu_former_position = pingu->local_position;

            //If is standing on solid
            if(pingu->IsOverlappingMovingSolid(pingu->local_position + Vector2f(0.0f,total_movement_y) + Vector2f(0.0f,1.0f), olc::WHITE, this)
                && !pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)
            ){
                pingu->local_position.y += total_movement_y;

                //To avoid rounding errors the pingu is pushed one pixel further if still in solid.
                if(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)){
                    pingu->local_position.y -= ufoMaths::PreciseSignFloat(total_movement_y);
                }
            
                pingu_was_moved = true;
            }
            if(ufoMaths::PreciseSignFloat(total_movement_y) == 0.0f) continue;

            //Pushes pingu if NOT on the slope
            while(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)){
                pingu->local_position.y += ufoMaths::PreciseSignFloat(total_movement_y);
                Console::PrintLine("MovingSolid pingu->local_position.y", total_movement_y, ufoMaths::Sign(total_movement_y));
                pingu_was_moved = true;
            }

            //This could be faulty. Resolving without pingu touching MovingSolid
            while(pingu->IsOverlapping(pingu->level, pingu->mask_decal, pingu->solid_layer, pingu->local_position, olc::WHITE)){
                pingu->local_position.y -= ufoMaths::PreciseSignFloat(total_movement_y);
                pingu_was_moved = true;
            }
        
            if(pingu_was_moved){
                pingu->OnMoved(this);
            }

        }
    //}

    //Console::Print("MovingSolid");

}