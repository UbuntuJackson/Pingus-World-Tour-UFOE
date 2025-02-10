#pragma once
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"

class Level;
class TuxTalePlayer;
class Level;
class TuxTaleLevel;

class FloatingBlock : public Actor{
public:
    TuxTaleLevel* level = nullptr;
    FloatingBlock(Vector2f _local_position);
    bool ready = false;

    void OnLevelEnter(Level* _level);
    virtual void OnUpdate();
    virtual void OnHit(TuxTalePlayer* _level);
    virtual void OnInRange(TuxTalePlayer* _player);
    virtual void OnOutOfRange(TuxTalePlayer* _player);
    std::string GetType();

};