#include <memory>
#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../json/json.h"
#include "../animation/animation.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "battle_player.h"
#include "tuxtale_level.h"
#include "../level/level.h"
#include "../keyboard/single_keyboard.h"
#include "../animation/animation.h"
#include "../animation/animated_sprite_reference.h"
#include "tuxtale_battle_hud.h"
#include "battle_participant.h"

BattlePlayer::BattlePlayer(Vector2f _local_position, TuxTaleHud* _tuxtale_hud) :
BattleParticipant(_local_position, _tuxtale_hud)
{

}
void BattlePlayer::OnLevelEnter(Level* _level){
    BattleParticipant::OnLevelEnter(_level);

    auto u_animation = std::make_unique<Animation>(Vector2f(0.0f, 0.0f));

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
}
void BattlePlayer::OnUpdate(){

    BattleParticipant::OnUpdate();

}

void BattlePlayer::OnTurn(){
    if(!can_move) return;

    if(SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed && !sword_anim && ready){
        sword_anim = AddChild<Animation>(Vector2f(16.0f, 0.0f));
        sword_anim->AddAnimationState(AnimatedSpriteReference(
            "tuxtale_sword",
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f, 16.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0,
            20.0f
        ));
        sword_anim->SetAnimation("tuxtale_sword");
        sword_anim->SetZIndex(3);
    }

    ready = true;

    if(sword_anim){
        if(sword_anim->cycle_count >= 1){
            sword_anim->QueueForPurge();
            sword_anim = nullptr;
            tuxtale_hud->state = TuxTaleHud::BattleModeStates::ACTION_SELECT;
            tuxtale_hud->wrap_menu_stack.clear();
            state = RETREAT;
            can_move = false;
        }
    }

    velocity.x += (float)(int(SingleKeyboard::Get().GetKey(olc::RIGHT).is_held) - int(SingleKeyboard::Get().GetKey(olc::LEFT).is_held)) * Engine::Get().Get().GetDeltaTime() * acceleration_x;

    float former_x_velocity = velocity.x;

    velocity.x -= ufoMaths::Sign(velocity.x) * Engine::Get().GetDeltaTime() * friction_x;
    if(former_x_velocity * velocity.x < 0.0f) velocity.x = 0.0f;

    
    if(SingleKeyboard::Get().GetKey(olc::Z).is_pressed && is_on_ground) velocity.y = initial_jump_speed;
    

    if(is_on_ground){
        if(SingleKeyboard::Get().GetKey(olc::Z).is_released) velocity.y*=0.5f;
    }

    velocity.y += gravity * Engine::Get().Get().GetDeltaTime();

    TuxTaleLevel* level = dynamic_cast<TuxTaleLevel*>(Engine::Get().current_level.get());

    if(level->PlaceFree(Rectangle(Vector2f(shape.position.x + velocity.x * Engine::Get().GetDeltaTime(), shape.position.y), shape.size)).place_free){
        local_position.x += velocity.x * Engine::Get().Get().GetDeltaTime();
    }
    else{
        while(level->PlaceFree(Rectangle(Vector2f(shape.position.x+ufoMaths::Sign(velocity.x), shape.position.y), shape.size)).place_free){
            local_position.x+=ufoMaths::Sign(velocity.x);
            Shape<Rectangle>::UpdateGlobalPosition(Vector2f(0.0f, 0.0f));
        }
        velocity.x = 0.0f;
    }

    Shape<Rectangle>::UpdateGlobalPosition(Vector2f(0.0f, 0.0f));

    is_on_ground = false;

    if(!level->PlaceFree(Rectangle(Vector2f(shape.position.x, shape.position.y + 8.0f), shape.size)).place_free){
        is_on_ground = true;
    }

    if(level->PlaceFree(Rectangle(Vector2f(shape.position.x, shape.position.y + velocity.y*Engine::Get().GetDeltaTime()), shape.size)).place_free){
        local_position.y += velocity.y * Engine::Get().Get().GetDeltaTime();
    }
    else{
        while(level->PlaceFree(Rectangle(Vector2f(shape.position.x, shape.position.y+ufoMaths::Sign(velocity.y)), shape.size)).place_free){
            local_position.y+=ufoMaths::Sign(velocity.y);
            Shape<Rectangle>::UpdateGlobalPosition(Vector2f(0.0f, 0.0f));
        }
        velocity.y = 0.0f;
    }
}