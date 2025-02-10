#include "../ufo_maths/ufo_maths.h"
#include "floating_block.h"
#include "tuxtale_player.h"
#include "tuxtale_level.h"
#include "coin_block.h"
#include "../sprite_reference/sprite_reference.h"
#include "coin_effect.h"
#include "../asset_manager/asset_manager.h"

CoinBlock::CoinBlock(Vector2f _local_position) : FloatingBlock(_local_position){

}

void
CoinBlock::OnLevelEnter(Level* _level){
    FloatingBlock::OnLevelEnter(_level);

    sprite_ref = AddChild<SpriteReference>(
        "coin_block",
        Vector2f(0.0f, -24.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(16.0f, 24.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );
}

void
CoinBlock::OnHit(TuxTalePlayer* _player){
    if(!empty){
        AssetManager::Get().PlayAudio("tuxtale_coin", false);
        _player->coin_count++;
        auto coin_effect = _player->level->NewActor<CoinEffect>(local_position-Vector2f(0.0f,24.0f));
        coin_effect->SetZIndex(5);
        sprite_ref->current_frame_index = 1;
        empty = true;
        sprite_ref->tint.a = 200;
    }
}