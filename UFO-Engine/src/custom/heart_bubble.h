#ifndef HEART_BUBBLE
#define HEART_BUBBLE

#include "../external/olcPixelGameEngine.h"
#include "../collision/collision_body.h"
#include "../shapes/circle.h"
#include "../sprite_reference/sprite_reference.h"
#include "../random/random_number_generator.h"
#include "../level/ufo_engine.h"
#include "frogatto_level.h"
#include "../ufo_clock/timer.h"
class HeartBubble : public CollisionBody{
public:
    Timer invincibility_timer;
    HeartBubble(olc::vf2d _position) : CollisionBody(_position){
        dynamic_cast<FrogattoLevel*>(Engine::Get().current_level.get())->heart_bubble_handles.push_back(this);
        AddChild(std::make_unique<SpriteReference>(
            "heart_bubble",
            olc::vf2d(-7.0f, -7.0f),
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(16.0f, 16.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0
                ));
        
        velocity.x = RandomNumberGenerator::Get().RandomFloat(-1.0f, 1.0f);
        invincibility_timer.Start(1000.0f); 
    }

    void OnLevelEnter(Level* _level){
        CollisionBody::OnLevelEnter(_level);

        SetShape(CollisionCircle(this,olc::vf2d(0.0f,0.0f), 7.0f),olc::vf2d(0.0f, 0.0f));
    }

    void OnUpdate(){
        CollisionBody::OnUpdate();
        velocity+=collision_normal*2.0f;
        velocity.y += 0.1;
    }

};

#endif