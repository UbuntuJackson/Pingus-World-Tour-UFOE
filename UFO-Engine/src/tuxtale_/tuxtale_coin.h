#ifndef TUXTALE_COIN
#define TUXTALE_COIN

#include <memory>
#include "../ufo_maths/ufo_maths.h"
#include "tuxtale_collectable.h"

class Animation;
class TuxTaleLevel;
class Level;
class TuxTalePlayer;

class TuxTaleCoin : public TuxTaleCollectable{
public:
    Animation* animation;
    TuxTaleLevel* level;
    Vector2f velocity;
    float velocity_z = 0.0f;
    float gravity = 40.0f;
    bool falling = false;
    float fade = 0.0f;
    bool fell_down_pit = false;

    TuxTaleCoin(Vector2f _position);

    void OnLevelEnter(Level* _level);

    void OnUpdate();

    void OnCollected(TuxTalePlayer* _player);
    
};

#endif