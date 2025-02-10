#include "../ufo_maths/ufo_maths.h"
#include "floating_block.h"
#include "tuxtale_player.h"
#include "tuxtale_level.h"
#include "save_block.h"
#include "../sprite_reference/sprite_reference.h"

SaveBlock::SaveBlock(Vector2f _local_position) : FloatingBlock(_local_position){

}

void
SaveBlock::OnLevelEnter(Level* _level){
    FloatingBlock::OnLevelEnter(_level);

    AddChild<SpriteReference>(
        "save_point",
        Vector2f(0.0f, -24.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(16.0f, 24.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );
}

void
SaveBlock::OnHit(TuxTalePlayer* _player){
    if(!_player->saved){
        _player->jump_velocity = 5.0f;
        Engine::Get().GetActiveProfile()->Save();
        _player->level->spawn_point = local_position+Vector2f(8.0f, 8.0f);
        _player->saved = true;
        AssetManager::Get().PlayAudio("saved", false);
    }

}

void
SaveBlock::OnInRange(TuxTalePlayer* _player){
    _player->can_save = true;
}