#pragma once

#include <shape.h>
#include <circle.h>
#include <ufo_maths.h>
#include <animated_sprite_reference.h>
#include <animation.h>
#include "pingus_level.h"

class HoneyCoin : public Shape<Circle>{
public:
    HoneyCoin(Vector2f _local_position) : Shape<Circle>(Circle(Vector2f(0.0f,0.0f),6.0f), _local_position, false, Vector2f(16.0f,16.0f)){

    }

    void OnLevelEnter(Level* _level){
        local_position.y -= 32.0f;

        auto anim = AddChild<Animation>(
            Vector2f(0.0f, 0.0f)
        );

        anim->AddAnimationState(AnimatedSpriteReference(
            "honey_coin",
            Vector2f(0.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0,
            5.0f
        ));
        anim->SetAnimation("honey_coin");

        dynamic_cast<PingusLevel*>(_level)->honey_coin_handles.push_back(this);
    }
};