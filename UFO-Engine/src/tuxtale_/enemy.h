#ifndef ENEMY_H
#define ENEMY_H

#include "../ufo_maths/ufo_maths.h"
#include "../level/actor.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"

class Level;
class TuxTalePlayer;

class Enemy : public Shape<Rectangle>{
public:
    Enemy(Vector2f _local_position, Rectangle _rectangle);

    void OnLevelEnter(Level* _level);

    virtual void OnHit(TuxTalePlayer* _player);

};

#endif