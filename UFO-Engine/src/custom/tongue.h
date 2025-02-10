#ifndef TONGUE_H
#define TONGUE_H

#include "../external/olcPixelGameEngine.h"
#include "../level/actor.h"
#include "../ufo_clock/timer.h"
#include "../shapes/circle.h"
#include "../shapes/shape.h"

class Level;
class SpriteReference;
class Frogatto;
class FrogattoLevel;
class Camera;

class Tongue : public Actor{
public:
    Timer life_time_timer;
    SpriteReference* sprite_reference;
    bool extended_phase_finished = false;
    float tongue_max_size = 60.0f;
    Shape<Circle>* hitbox;
    Frogatto* frogatto;
    FrogattoLevel* level;
    olc::vf2d direction;

    Tongue(olc::vf2d _position, Frogatto* _frogatto, olc::vf2d _direction);

    void OnLevelEnter(Level* _level);

    void OnUpdate();

    void OnDraw(Camera* _camera);
};

#endif