#include "../level/level.h"
#include "tuxtale_level.h"
#include "tuxtale_player.h"
#include "enemy.h"

Enemy::Enemy(Vector2f _local_position, Rectangle _rectangle) : Shape<Rectangle>(_rectangle,_local_position){

}

void Enemy::OnLevelEnter(Level* _level){
    TuxTaleLevel* level = dynamic_cast<TuxTaleLevel*>(_level);

    level->enemy_handles.push_back(this);
}

void Enemy::OnHit(TuxTalePlayer* _player){

}