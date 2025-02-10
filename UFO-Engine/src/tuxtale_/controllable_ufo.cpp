#include <memory>
#include <cmath>
#include "../shapes/circle.h"
#include "../shapes/shape.h"
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../sprite_reference/sprite_reference.h"
#include "../keyboard/single_keyboard.h"
#include "../external/olcPixelGameEngine.h"
#include "tuxtale_level.h"
#include "controllable_ufo.h"
#include "../ufo_clock/timer.h"
#include "../drawing_system/drawing_system.h"
#include "../asset_manager/asset_manager.h"

ControllableUFO::ControllableUFO(Vector2f _local_position) : Actor(_local_position){

}

void ControllableUFO::OnLevelEnter(Level* _level){
    //AddChild<Shape<Circle>>(Circle(Vector2f(0.0f, 0.0f), 16.0f), Vector2f(1.0f, 0.0f), false, Vector2f(0.0f, 0.0f));
    //AddChild<Shape<Circle>>(Circle(Vector2f(0.0f, 0.0f), 16.0f), Vector2f(16.0f-1.0f, 0.0f), false, Vector2f(0.0f, 0.0f));
    sprite_ref = AddChild<SpriteReference>("controllable_ufo",
        Vector2f(0.0f, 0.0f),
        Vector2f(24.0f, 24.0f),
        Vector2f(48.0f, 48.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );

    level = dynamic_cast<TuxTaleLevel*>(Engine::Get().current_level.get());

    level->vehicle_handles.push_back(this);
    
}

void ControllableUFO::OnUpdate(){
    if(!vehicle_active){
        sprite_ref->current_frame_index = 3.0f;
        return;
    }
    
    if(!is_dashing){
        if(std::abs(velocity.x) < max_speed) velocity.x += (float)(int(SingleKeyboard::Get().GetKey(olc::RIGHT).is_held) - int(SingleKeyboard::Get().GetKey(olc::LEFT).is_held)) * Engine::Get().Get().GetDeltaTime() * acceleration;

        float former_x_velocity = velocity.x;

        velocity.x -= ufoMaths::Sign(velocity.x) * Engine::Get().GetDeltaTime() * friction;
        if(former_x_velocity * velocity.x < 0.0f) velocity.x = 0.0f;

        if(std::abs(velocity.y) < max_speed) velocity.y += (float)(int(SingleKeyboard::Get().GetKey(olc::DOWN).is_held) - int(SingleKeyboard::Get().GetKey(olc::UP).is_held)) * Engine::Get().Get().GetDeltaTime() * acceleration;

        float former_y_velocity = velocity.y;

        velocity.y -= ufoMaths::Sign(velocity.y) * Engine::Get().GetDeltaTime() * friction;
        if(former_y_velocity * velocity.y < 0.0f) velocity.y = 0.0f;
    }

    if(std::abs(velocity.y) > 0.01f){
        if(velocity.y > 0.0f) sprite_ref->current_frame_index = 0.0f;
        if(velocity.y < 0.0f) sprite_ref->current_frame_index = 1.0f;
    }
    else if(velocity.x != 0.0f){
        sprite_ref->scale.x = -ufoMaths::Sign(velocity.x);
        sprite_ref->current_frame_index = 2.0f;
    }

    TuxTaleLevel* level = dynamic_cast<TuxTaleLevel*>(Engine::Get().current_level.get());

    if(level->PlaceFreeControllableUFO(Circle(Vector2f(local_position.x + velocity.x * Engine::Get().GetDeltaTime(), local_position.y), 16.0f)).place_free){
        local_position.x += velocity.x * Engine::Get().Get().GetDeltaTime();
    }
    else{
        Console::Out("Place not free");
        while(level->PlaceFreeControllableUFO(Circle(Vector2f(local_position.x+ufoMaths::Sign(velocity.x), local_position.y), 16.0f)).place_free){
            local_position.x+=ufoMaths::Sign(velocity.x);
        }
        velocity.x = 0.0f;
    }

    if(level->PlaceFreeControllableUFO(Circle(Vector2f(local_position.x, local_position.y + velocity.y*Engine::Get().GetDeltaTime()), 16.0f)).place_free){
        local_position.y += velocity.y * Engine::Get().Get().GetDeltaTime();
    }
    else{
        while(level->PlaceFreeControllableUFO(Circle(Vector2f(local_position.x, local_position.y+ufoMaths::Sign(velocity.y)), 16.0f)).place_free){
            local_position.y+=ufoMaths::Sign(velocity.y);
        }
        velocity.y = 0.0f;
    }

    if(SingleKeyboard::Get().GetKey(olc::X).is_pressed && !is_dashing){
        if(velocity.mag() != 0.0f){
            velocity_before_dash = velocity;
            velocity = velocity.norm()*700.0f;
            is_dashing = true;
            dash_timer.Start(100.0f);
            AssetManager::Get().PlayAudio("ufo_dash", false);
        }
    }

    if(is_dashing){
        auto spr = level->NewActor<SpriteReference>("controllable_ufo",
            local_position,
            Vector2f(24.0f, 24.0f),
            Vector2f(48.0f, 48.0f),
            Vector2f(1.0f, 1.0f),
            0.0f
        );
        spr->tint.r = (int)red_shift;
        spr->tint.b = 255-(int)red_shift;
        spr->tint.g = 255-(int)red_shift;
        spr->tint.a = 255-(int)(red_shift*0.75f);
        red_shift -= 1000.0f * Engine::Get().GetDeltaTime();
        spr->current_frame_index = sprite_ref->current_frame_index;
        
        sprite_references_with_time_limit.push_back(spr->id);
    }

    if(dash_timer.GetTimeLeft() <= 0.0f && is_dashing){
        velocity = {0.0f,0.0f};
        is_dashing = false;
        velocity = velocity_before_dash;
        red_shift = 255.0f;
    }

    if(dash_timer.GetTimeLeft() <= -200.0f){
        for(int i : sprite_references_with_time_limit){
            level->QueueForPurge(i);
            dash_timer.Stop();
        }
        sprite_references_with_time_limit.clear();
    }
}

std::unique_ptr<ControllableUFO> ControllableUFO::Load(Json* _json){
    return std::make_unique<ControllableUFO>(Vector2f(_json->GetAsInt("x"),_json->GetAsInt("y")));
}

void ControllableUFO::OnWidgetDraw(){
    if(vehicle_active){
        
        std::string text = "Press [X] to 'approach' light speed!";
        int len = text.size();
        float sx = Engine::Get().pixel_game_engine.GetScreenSize().x/2.0f - (float)len*8.0f/2.0f;
        float sy = Engine::Get().pixel_game_engine.GetScreenSize().y - 20.0f;

        Engine::Get().pixel_game_engine.DrawStringDecal(Vector2f(sx+1.0f,sy+1.0f), text, olc::BLACK);
        Engine::Get().pixel_game_engine.DrawStringDecal(Vector2f(sx,sy), text);
        
    }
}