#include "ant.h"
#include "../collision/collision_body.h"
#include "../shapes/circle.h"
#include "../json/json.h"
#include "../animation/animated_sprite_reference.h"
#include "frogatto_level.h"
#include "frogatto.h"
#include "edible.h"

Ant::Ant(olc::vf2d _position) : CollisionBody(_position){
    auto u_animation = std::make_unique<Animation>(olc::vf2d(0.0f,0.0f));
    
    u_animation->AddAnimationState(
        AnimatedSpriteReference("yellow_ant", olc::vf2d(0.0f, 0.0f), olc::vf2d(8.0f, 8.0f), olc::vf2d(16.0f, 16.0f), olc::vf2d(1.0f, 1.0f), 0.0f, 0, 10.0f)
    );
    u_animation->SetAnimation("yellow_ant");

    animation = u_animation.get();

    AddChild(std::move(u_animation));
}

void
Ant::OnLevelEnter(Level* _level){
    CollisionBody::OnLevelEnter(_level);

    SetShape(CollisionCircle(this,olc::vf2d(0.0f, 0.0f),7.0f),olc::vf2d(0.0f, 0.0f));

    auto level = dynamic_cast<FrogattoLevel*>(_level);
    level->enemy_handles.push_back(this);

    AddChild(std::make_unique<TEdible<Ant>>(this, shape));

    velocity.x = 1.0f;
}

void
Ant::OnEaten(Frogatto* _frogatto){
    _frogatto->eaten_actor_spawner = [](Frogatto* _ffrogatto){
        auto new_ant = std::make_unique<Ant>(_ffrogatto->position+olc::vf2d(8.0f*_ffrogatto->face_direction, -1.0f));
        new_ant->velocity.x = _ffrogatto->face_direction;
        new_ant->velocity.y = -2.0f;
        return std::move(new_ant);
    };
}

void Ant::Accelerate(){

    const int MAX_SPEED = 3.0f;
    const int GROUND_FRICTION = 0.01f;

    int ground_acceleration = 1.0f;

    //if(collision_normal.x != 0.0f) Console::Out("Collision normal .x:", collision_normal.x);

    if(collision_normal.x > 0.0f) velocity.x = 1.0f;
    if(collision_normal.x < 0.0f) velocity.x = -1.0f;

    //if(MAX_SPEED > std::abs(velocity.x)) velocity.x += ground_acceleration;

    float hspeed_prefriction = velocity.x;
    velocity.x-=GROUND_FRICTION*ufoMaths::Sign(velocity.x);

    //if(velocity.x * hspeed_prefriction < 0.0f) velocity.x = 0.0f;

    velocity.y+=0.1f;
}

void Ant::OnUpdate(){
    CollisionBody::OnUpdate();
    
    switch(state){
        case State::UPSIDE_DOWN:{
            //animation->SetAnimation("yellow_ant_upside_down");
        }
        break;
        case State::WALKING:{
            animation->SetAnimation("yellow_ant");
        }
        break;
    }

}

std::unique_ptr<Ant> Ant::Load(Json& _json){
    float x = (float)_json.GetAsInt("x");
    float y = (float)_json.GetAsInt("y");
    return std::make_unique<Ant>(olc::vf2d(x, y));
}