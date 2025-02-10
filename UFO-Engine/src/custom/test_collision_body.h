#ifndef TEST_COLLITION_BODY
#define TEST_COLLITION_BODY
#include <memory>
#include "../json/json.h"
#include "../collision/collision_body.h"
#include "../external/olcPixelGameEngine.h"
#include "../level/level.h"
#include "../shapes/collision_pill.h"
#include "../shapes/collision_circle.h"

class TestCollisionBody : public CollisionBody{
public:
    TestCollisionBody(olc::vf2d _position) : CollisionBody(_position){}

    void OnLevelEnter(Level* _level){
        CollisionBody::OnLevelEnter(_level);

        //SetShape(CollisionPill(olc::vf2d(0.0f, 0.0f), 5.0f, 10.0f, this), olc::vf2d(0.0f, 0.0f));
        SetShape(CollisionCircle(this, olc::vf2d(0.0f, 0.0f), 5.0f), olc::vf2d(0.0f, 0.0f));
    }

    olc::vf2d OnAccelerate(){
        int direction_x = (int)SingleKeyboard::Get().GetKey(olc::RIGHT).is_held - (int)SingleKeyboard::Get().GetKey(olc::LEFT).is_held;
        int direction_y = (int)SingleKeyboard::Get().GetKey(olc::DOWN).is_held - (int)SingleKeyboard::Get().GetKey(olc::UP).is_held;
        return olc::vf2d(direction_x*acceleration.x*60.0f, direction_y*acceleration.y*60.0f);
    }

    void OnUpdate(){
        int direction_x = (int)SingleKeyboard::Get().GetKey(olc::RIGHT).is_held - (int)SingleKeyboard::Get().GetKey(olc::LEFT).is_held;
        int direction_y = (int)SingleKeyboard::Get().GetKey(olc::DOWN).is_held - (int)SingleKeyboard::Get().GetKey(olc::UP).is_held;
        
        CollisionBody::OnUpdate();
    }

    static std::unique_ptr<TestCollisionBody> Load(Json& _object){
        return std::make_unique<TestCollisionBody>(olc::vf2d((float)_object.GetAsInt("x"),(float)_object.GetAsInt("y")));
    }
};

#endif