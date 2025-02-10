#ifndef COIN_EFFECT_H
#define COIN_EFFECT_H

#include "tuxtale_coin.h"
#include "../level/level.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"

using namespace ufoMaths;

class CoinEffect : public TuxTaleCoin{
public:
    CoinEffect(Vector2f _local_position) : TuxTaleCoin(_local_position){
        is_collectable = false;
        z_index = 2;
    }

    void OnUpdate(){
        animation->current_animation_state->tint.a -= (0.1f * Engine::Get().GetDeltaTime());
        local_position.y-=(90.0f*Engine::Get().GetDeltaTime());
    }


};

#endif