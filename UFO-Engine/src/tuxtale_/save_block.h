#pragma once
#include "floating_block.h"
#include "../ufo_maths/ufo_maths.h"

class Level;
class TuxTalePlayer;
class Level;

class SaveBlock : public FloatingBlock{
public:
    SaveBlock(Vector2f);
    void OnLevelEnter(Level* _level);
    void OnHit(TuxTalePlayer* _player);
    void OnInRange(TuxTalePlayer* _player);

};