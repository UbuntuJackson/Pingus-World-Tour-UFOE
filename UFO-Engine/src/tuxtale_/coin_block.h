#pragma once
#include "floating_block.h"
#include "../ufo_maths/ufo_maths.h"

class Level;
class TuxTalePlayer;
class Level;
class SpriteReference;

class CoinBlock : public FloatingBlock{
public:
    SpriteReference* sprite_ref;
    bool empty = false;

    CoinBlock(Vector2f _local_position);

    void OnLevelEnter(Level* _level);
    virtual void OnHit(TuxTalePlayer* _level);

};