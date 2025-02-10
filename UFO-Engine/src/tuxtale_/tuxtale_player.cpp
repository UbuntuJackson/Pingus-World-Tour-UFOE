#include <memory>
#include <string>
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../console/console.h"
#include "../json/json.h"
#include "../keyboard/single_keyboard.h"
#include "../level/ufo_engine.h"
#include "../camera/camera.h"
#include "../animation/animation.h"
#include "../animation/animated_sprite_reference.h"
#include "tuxtale_level.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "../sprite_reference/sprite_reference.h"
#include "../level/level_trigger.h"
#include "../level/spawn_point.h"
#include "../asset_manager/asset_manager.h"
#include "enemy.h"
#include "tuxtale_coin.h"
#include "coin_effect.h"
#include "../widgets/one_dimensional_text_box.h"
#include "tuxtale_player.h"
#include "talkable.h"
#include "pause_menu.h"
#include "../tilemap/tile_map.h"
#include "../file/file_menu.h"
#include "dying_sequence.h"
#include "floating_block.h"
#include "controllable_ufo.h"

using namespace ufoMaths;

TuxTalePlayer::TuxTalePlayer(Vector2f _position) : Shape<Rectangle>(Rectangle(Vector2f(0.0f, 0.0f), Vector2f(14.0f, 7.0f)),_position, false, Vector2f(-7.0f, -0.0f)){
    
}

void TuxTalePlayer::OnLevelEnter(Level* _level){
    Shape<Rectangle>::OnLevelEnter(_level);

    //Sets z_index to 1 so that tux appears on top of things like enemies.
    z_index = 1;

    _level->spawn_point = local_position;

    auto u_camera = std::make_unique<Camera>(Vector2f(0.0f, 0.0f));
    u_camera->clamp = true;
    camera = u_camera.get();
    AddChild(std::move(u_camera));

    auto u_shadow = std::make_unique<SpriteReference>(
        "shadow",
        Vector2f(0.0f, 0.0f),
        Vector2f(8.0f, 8.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(1.0f, 1.0f),
        0,
        0
    );

    shadow = u_shadow.get();

    AddChild(std::move(u_shadow));

    auto u_animation = std::make_unique<Animation>(Vector2f(0.0f, 0.0f));

    u_animation->AddAnimationState(AnimatedSpriteReference(
        "walk_up",
        Vector2f(0.0f, 0.0f),
        Vector2f(8.0f, 8.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(1.0f, 1.0f),
        0,
        0,
        5.0f
    ));

    u_animation->AddAnimationState(AnimatedSpriteReference(
        "walk_down",
        Vector2f(0.0f, 0.0f),
        Vector2f(8.0f, 8.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(1.0f, 1.0f),
        0,
        0,
        5.0f
    ));

    u_animation->AddAnimationState(AnimatedSpriteReference(
        "walk_horizontal",
        Vector2f(0.0f, 0.0f),
        Vector2f(8.0f, 8.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(1.0f, 1.0f),
        0,
        0,
        5.0f
    ));

    animation = u_animation.get();

    u_animation->SetAnimation("walk_horizontal");

    AddChild(std::move(u_animation));

    _level->savable_actor_handles.push_back(this);

    level = dynamic_cast<TuxTaleLevel*>(_level);

    level->transitionable_actor_handles.push_back(this);

}

void TuxTalePlayer::OnStart(Level* _level){
    gravity*=level->gravity_scale;
    Console::Out("Setting gravity scale to", gravity);
}

void TuxTalePlayer::OnTransition(Level* _former_level){
    if(level->spawn_mode == SpawnMode::SPAWN_POINT){
        local_position = level->spawn_point;
    }
}

void TuxTalePlayer::OnUpdate(){

    Shape<Rectangle>::OnUpdate();

    if(SingleKeyboard::Get().GetKey(olc::ESCAPE).is_pressed){
        level->NewActor(PauseMenu());
    }

    if(hit_enemy) return;

    bool took_step = false;

    if(SingleKeyboard::Get().GetKey(olc::Z).is_pressed && animation->local_position.y >= 0.0f){
        jump_velocity = initial_jump_velocity;
        AssetManager::Get().PlayAudio("jump", false);
    }

    animation->local_position.y+=jump_velocity*Engine::Get().GetDeltaTime();

    jump_velocity += gravity*Engine::Get().GetDeltaTime();

    if(animation->local_position.y > 0.0f){
        animation->local_position.y = 0.0f;
    }

    if(animation->local_position.y < 0.0f){
        walking_speed = 85.0f;
        shadow->visible = true;
        on_ground = false;
    }
    else{
        walking_speed = 80.0f;
        shadow->visible = false;
        on_ground = true;
    }

    for(auto&& vehicle : level->vehicle_handles){
        if(!on_ground){
            vehicle->vehicle_active = false;
            animation->current_animation_state->visible = true;
        }
        else if(ufoMaths::RectangleVsCircle(shape, Circle(vehicle->local_position, 16.0f))){
            local_position = vehicle->local_position;
            vehicle->vehicle_active = true;
            animation->current_animation_state->visible = false;
            on_ground = true;
            return;
        }
    }

    float step_x = 0.0f;
    float step_y = 0.0f;

    if(SingleKeyboard::Get().GetKey(olc::LEFT).is_held){
        step_x = -walking_speed * Engine::Get().GetDeltaTime();
        animation->current_animation_state->scale.x = -1.0f;
        animation->SetAnimation("walk_horizontal");
        took_step = true;
    }
    if(SingleKeyboard::Get().GetKey(olc::RIGHT).is_held){
        step_x = walking_speed * Engine::Get().GetDeltaTime();
        animation->current_animation_state->scale.x = 1.0f;
        animation->SetAnimation("walk_horizontal");
        took_step = true;
    }

    if(level->PlaceFree(Rectangle(Vector2f(shape.position.x + step_x, shape.position.y), shape.size)).place_free){
        local_position.x += step_x;
    }
    else{
        while(level->PlaceFree(Rectangle(Vector2f(shape.position.x+ufoMaths::Sign(step_x), shape.position.y), shape.size)).place_free){
            local_position.x+=ufoMaths::Sign(step_x);
            Shape<Rectangle>::UpdateGlobalPosition(Vector2f(0.0f, 0.0f));
        }
    }

    Shape<Rectangle>::UpdateGlobalPosition(Vector2f(0.0f, 0.0f));

    if(SingleKeyboard::Get().GetKey(olc::UP).is_held){
        step_y = -walking_speed * Engine::Get().GetDeltaTime();
        animation->SetAnimation("walk_up");
        took_step = true;
    }
    if(SingleKeyboard::Get().GetKey(olc::DOWN).is_held){
        step_y = walking_speed * Engine::Get().GetDeltaTime();
        animation->SetAnimation("walk_down");
        took_step = true;
    }

    //camera->local_position_set_elsewhere = false;

    /*if(step_x != 0.0f){
        camera->local_position.x = camera_offset_x;
        
        camera_offset_x+=step_x;
    }
    camera->local_position.y += step_y;*/

    if(level->PlaceFree(Rectangle(Vector2f(shape.position.x, shape.position.y + step_y), shape.size)).place_free){
        local_position.y += step_y;
    }
    else{
        while(level->PlaceFree(Rectangle(Vector2f(shape.position.x, shape.position.y+ufoMaths::Sign(step_y)), shape.size)).place_free){
            local_position.y+=ufoMaths::Sign(step_y);
            Shape<Rectangle>::UpdateGlobalPosition(Vector2f(0.0f, 0.0f));
        }
    }

    if(on_ground){
        bool fell_in_pit = true;

        TileCollisionData tile_collision_data = level->PlaceFree(shape);

        for(const int tile : tile_collision_data.tiles){
            if(tile != 12) fell_in_pit = false;
        }

        if(fell_in_pit) DieFromFall();
    }

    if(!took_step){
        animation->current_animation_state->current_frame_index = 0.0f;
    }

    

    if(!took_step_last_frame && took_step){
        animation->current_animation_state->current_frame_index = 1.0f;
    }

    took_step_last_frame = took_step;

    can_save = false;

    for(const auto& floating_block : level->floating_block_handles){
        
        if(ufoMaths::Distance2(local_position, floating_block->local_position+Vector2f(8.0f, 5.0f)) < 8.0f){
            floating_block->OnInRange(this);
            
            if(animation->local_position.y < -5.0f){
                if(!bumped_head){
                    
                    floating_block->OnHit(this);
                    jump_velocity = 5.0f;
                    AssetManager::Get().PlayAudio("bump_head", false);
                    bumped_head = true;
                }
            }
            else{
                bumped_head = false;
            }
        }
        else{
            floating_block->OnOutOfRange(this);
        }
    }

    /*for(const auto& save_point : level->save_point_handles){
        if(ufoMaths::Distance2(local_position, save_point->local_position+Vector2f(8.0f, 5.0f)) < 8.0f){
            if(animation->local_position.y < -5.0f){
                if(!bumped_head){
                    jump_velocity = 5.0f;
                    AssetManager::Get().PlayAudio("bump_head", false);
                    bumped_head = true;
                }
            }
            else{
                bumped_head = false;
            }
            
            if(animation->local_position.y < -5.0f && !saved){
                jump_velocity = 5.0f;
                Engine::Get().GetActiveProfile()->Save();
                level->spawn_point = save_point->local_position+Vector2f(8.0f, 8.0f);
                saved = true;
                AssetManager::Get().PlayAudio("saved", false);
            }

            can_save = true;
        }
    }*/

    if(took_step && !can_save) saved = false;

    for(const auto& enemy : level->enemy_handles){
        if(ufoMaths::RectangleVsRectangle(shape, enemy->shape) && on_ground){
            enemy->OnHit(this);
            AssetManager::Get().PlayAudio("hurt", false);
            hit_enemy = true;
        }
    }

    for(auto&& collectable : level->collectable_handles){
        if(ufoMaths::RectangleVsCircle(shape, collectable->shape) && on_ground){
            collectable->OnCollected(this);
        }
    }

    for(const auto& trigger : level->level_trigger_handles){
        if(ufoMaths::RectangleVsPoint(trigger->shape,local_position)){
            level->hit_level_triggers.push_back(trigger);
            
            std::unique_ptr<TuxTaleLevel> u_level = std::make_unique<TuxTaleLevel>();

            u_level->spawn_mode = SpawnMode::SPAWN_POINT;
            u_level->spawn_point_name = trigger->spawn_point_name;

            Engine::Get().GoToLevel(std::move(u_level),trigger->path);
        }
    }

    for(auto&& talkable : level->talkable_handles){
        if(ufoMaths::RectangleVsCircle(shape, talkable->shape) && SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed){
            
            talkable->OnHit(this);
        }
    }

    std::vector<TileMap*> tilemaps = level->GetActorsOfType<TileMap>();

    float former_z_index = z_index;

    for(const auto& tilemap : tilemaps){
        if(tilemap->z_index < 2) continue;
        
        bool stop_iterating = false;
        for(int yy = -1; yy <= 1; yy++){
            for(int xx = -1; xx <= 1; xx++){
                int tile_id = tilemap->GetTileID_AtLevelPosition(GetGlobalPosition()+Vector2f(xx*16.0f,yy*16.0f));

                float tile_position_x = std::floor(GetGlobalPosition().x/16.0f+xx) * 16.0f;
                float tile_position_y = std::floor(GetGlobalPosition().y/16.0f+yy) * 16.0f;

                if(ufoMaths::RectangleVsRectangle(shape, Rectangle(Vector2f(tile_position_x, tile_position_y), Vector2f(16.0f, 16.0f)))){
                    if(tile_id == 0){
                        z_index = 3;
                        
                    }
                    else{
                        z_index = 1;
                        stop_iterating = true;
                        break;
                    }
                }
            }
            if(stop_iterating) break;
        }
    }

    if(former_z_index != z_index) level->should_resort_after_z_index = true;

}

void TuxTalePlayer::OnPaused(){
    
}

std::unique_ptr<TuxTalePlayer> TuxTalePlayer::Load(Json& _json){
    return std::make_unique<TuxTalePlayer>(Vector2f((float)_json.GetAsInt("x"), (float)_json.GetAsInt("y")-16.0f));
}

void TuxTalePlayer::OnLoad(Json* _current_save_file){
    
}

void TuxTalePlayer::OnSave(Json* _current_save_file){

    Console::Out("Player::OnSave");

    Json player_data = Json::NewObject();
    
    player_data.Set("x", Json::NewObject(local_position.x));
    player_data.Set("y", Json::NewObject(local_position.y));

    _current_save_file->Set("player_data", player_data);

}

void TuxTalePlayer::DieFromFall(){
    if(died) return;
    //Engine::Get().GetActiveProfile()->Load();
    level->NewActor<DyingSequence>(GetGlobalPosition());

    QueueForPurge();

    for(int i = 0; i < coin_count; i++){
        auto coin = level->NewActor<TuxTaleCoin>(GetGlobalPosition()-Vector2f(8.0f,8.0f));
    
        float angle = i* 2*ufoMaths::PI / coin_count;

        coin->falling = true;
        coin->velocity.y = -25.0f * ufoMaths::Sin(angle);
        coin->velocity.x = -25.0f * ufoMaths::Cos(angle);
        coin->velocity_z = -30.0f;
    }

    Console::Out("TuxTalePlayer::DieFromFall");
    died = true;
    AssetManager::Get().PlayAudio("hurt", false);
}

void TuxTalePlayer::OnWidgetDraw(){

    if(can_save){
        if(!saved){
            std::string text = "Bump this block to save!";
            int len = text.size();
            float sx = Engine::Get().pixel_game_engine.GetScreenSize().x/2.0f - (float)len*8.0f/2.0f;
            float sy = Engine::Get().pixel_game_engine.GetScreenSize().y - 20.0f;

            Engine::Get().pixel_game_engine.DrawStringDecal(Vector2f(sx+1.0f,sy+1.0f), text, olc::BLACK);
            Engine::Get().pixel_game_engine.DrawStringDecal(Vector2f(sx,sy), text);
        }
        else{
            int len = std::string("File saved [v]").size();
            float sx = Engine::Get().pixel_game_engine.GetScreenSize().x/2.0f - (float)len*8.0f/2.0f;
            float sy = Engine::Get().pixel_game_engine.GetScreenSize().y - 20.0f;

            Engine::Get().pixel_game_engine.DrawStringDecal(Vector2f(sx+1.0f,sy+1.0f), "File saved [v]", olc::BLACK);
            Engine::Get().pixel_game_engine.DrawStringDecal(Vector2f(sx,sy), "File saved [v]");
        }
    }
}
