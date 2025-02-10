#ifndef ANT
#define ANT

#include "../collision/collision_body.h"
#include "../external/olcPixelGameEngine.h"
#include "../animation/animation.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/circle.h"
#include "../json/json.h"

class Frogatto;
class Animation;

class Ant : public CollisionBody{
public:
    enum State{
        UPSIDE_DOWN,
        WALKING
    };

    int state = State::UPSIDE_DOWN;
    Animation* animation;
    
    Ant(olc::vf2d _position);
    void OnLevelEnter(Level* _level);
    
    void Accelerate();
    void OnUpdate();
    void OnEaten(Frogatto* _frogatto);
    static std::unique_ptr<Ant> Load(Json& _json);
};

#endif