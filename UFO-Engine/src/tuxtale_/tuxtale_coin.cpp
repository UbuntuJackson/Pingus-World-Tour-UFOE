#include <memory>
#include "../json/json.h"
#include "../animation/animated_sprite_reference.h"
#include "../animation/animation.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"
#include "../ufo_maths/ufo_maths.h"
#include "tuxtale_level.h"
#include "tuxtale_collectable.h"
#include "coin_effect.h"
#include "tuxtale_coin.h"

TuxTaleCoin::TuxTaleCoin(Vector2f _position) : TuxTaleCollectable(_position){
    
}

void TuxTaleCoin::OnLevelEnter(Level* _level){
    TuxTaleCollectable::OnLevelEnter(_level);

    auto u_coin_animation = std::make_unique<Animation>(Vector2f(0.0f, 0.0f));

    u_coin_animation->AddAnimationState(AnimatedSpriteReference(
        "tuxtale_coin",
        olc::vf2d(0.0f, 0.0f),
        olc::vf2d(0.0f,0.0f),
        olc::vf2d(16.0f,16.0f),
        olc::vf2d(1.0f, 1.0f),
        0.0f,
        0,
        8.0f
    ));
    u_coin_animation->SetAnimation("tuxtale_coin");

    animation = u_coin_animation.get();

    AddChild(std::move(u_coin_animation));

    level = dynamic_cast<TuxTaleLevel*>(_level);
}

void TuxTaleCoin::OnUpdate(){
    Shape<Circle>::OnUpdate();

    if(falling){
        velocity_z += gravity * Engine::Get().GetDeltaTime();
        animation->local_position.y += velocity_z * Engine::Get().GetDeltaTime();
        local_position += velocity * Engine::Get().GetDeltaTime();

        if(animation->local_position.y > 0.0f){
            animation->current_animation_state->tint.a = 255 - int(fade);
            fade+=1000.0f*Engine::Get().GetDeltaTime();

            std::vector<int> tiles = level->PlaceFree(Rectangle(local_position, 2.0f*Vector2f(shape.radius, shape.radius))).tiles;
            for(int tile : tiles){
                if(tile == 12){
                    fell_down_pit = true;
                    velocity.x = 0.0f;
                }
            }

            if(!fell_down_pit){
                falling = false;
                velocity_z = 0.0f;
                velocity = {0.0f, 0.0f};
            }

        }

        if(animation->local_position.y > 20.0f){

            if(fell_down_pit) QueueForPurge();

        }
    }
}

void TuxTaleCoin::OnCollected(TuxTalePlayer* _player){
    AssetManager::Get().PlayAudio("tuxtale_coin", false);
    level->NewActor(std::make_unique<CoinEffect>(GetGlobalPosition()));
    _player->coin_count++;
    QueueForPurge();
}