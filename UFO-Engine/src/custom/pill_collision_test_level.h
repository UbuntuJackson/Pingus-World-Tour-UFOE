#ifndef PILL_COLLISION_TEST_LEVEL
#define PILL_COLLISION_TEST_LEVEL

#include <memory>
#include "../level/level.h"
#include "../console/console.h"
#include "../mouse/mouse.h"
#include "../shapes/shape.h"
#include "../shapes/collision_pill.h"
#include "../external/olcPixelGameEngine.h"
#include "../custom/test_collision_body.h"

class PillCollisionTestLevel : public Level{

    void OnUpdate(){
        Level::OnUpdate();

        GetActiveCamera()->clamp = false;

        if(Mouse::GetLeftButton().is_pressed){
            olc::vf2d spawn_position = GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());

            //auto cb = std::make_unique<TestCollisionBody>(spawn_position);

            //cb->SetShape(CollisionPill(olc::vf2d(0.0f, 0.0f), 5.0f, 10.0f, cb.get()), olc::vf2d(0.0f, 0.0f));

            NewActor(std::make_unique<TestCollisionBody>(spawn_position));
        }
    }

};

#endif