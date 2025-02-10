#pragma once
#include "../ufo_maths/ufo_maths.h"
#include "tuxtale_level.h"
#include "../level/level.h"
#include "tuxtale_collectable.h"
#include "../animation/animated_sprite_reference.h"
#include "../animation/animation.h"
#include "../asset_manager/asset_manager.h"
#include "tuxtale_player.h"

class TuxTaleHeart : public TuxTaleCollectable{
public:
    Animation* heart_animation = nullptr;
    bool collected = false;
    float f_tint = 255.0f;
    TuxTaleHeart(Vector2f _local_position) : TuxTaleCollectable(_local_position){
        
    }

    void OnLevelEnter(Level* _level){
        TuxTaleCollectable::OnLevelEnter(_level);

        heart_animation = AddChild<Animation>(Vector2f(8.0f, 8.0f));
        heart_animation->AddAnimationState(AnimatedSpriteReference(
            "tuxtale_heart",
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(8.0f,8.0f),
            olc::vf2d(16.0f,18.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f,
            0,
            8.0f
        ));
        heart_animation->SetAnimation("tuxtale_heart");
    }

    void OnUpdate(){
        TuxTaleCollectable::OnUpdate();

        if(collected){
            heart_animation->current_animation_state->scale.x += 1.0f * Engine::Get().GetDeltaTime();
            heart_animation->current_animation_state->scale.y += 1.0f * Engine::Get().GetDeltaTime();
            f_tint -= 100.0f * Engine::Get().GetDeltaTime();
            heart_animation->current_animation_state->tint.a = (int)f_tint;
            if(f_tint <= 0.0f) QueueForPurge();
        }
    }

    void OnCollected(TuxTalePlayer* _player){
        if(collected) return;
        SetZIndex(3);
        
        collected = true;
        AssetManager::Get().PlayAudio("got_heart", false);
    }

};