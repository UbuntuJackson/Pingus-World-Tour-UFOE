#include <memory>
#include "balloon.h"
#include <cmath>
#include "../external/olcPixelGameEngine.h"
#include "../level/actor.h"
#include "../shapes/shape.h"
#include "../level/ufo_engine.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/circle.h"
#include "../json/json.h"
#include "frogatto_level.h"
#include "balloon_pop.h"
#include "heart_bubble.h"

Balloon::Balloon(olc::vf2d _position, int _balloon_type) :
    Shape<Circle>(Circle(olc::vf2d(0.0f, 0.0f), 9.0f), _position),balloon_type{_balloon_type}{
    
}

void Balloon::OnLevelEnter(Level* _level){
    dynamic_cast<FrogattoLevel*>(_level)->balloon_handles.push_back(this);
    auto spr = std::make_unique<SpriteReference>(
        "baloon",
        olc::vf2d(-9.0f,-9.0f),
        olc::vf2d(0.0f,0.0f),
        olc::vf2d(18.0f,48.0f),
        olc::vf2d(1.0f, 1.0f),
        0.0f,
        0
    );
    spr->current_frame_index = balloon_type;
    
    AddChild(std::move(spr));
}

void Balloon::OnUpdate(){
    Shape<Circle>::OnUpdate();
    if(Engine::Get().vsync){
        count+=0.01f;
        velocity.y=count*1.0f;
        local_position.y+=std::sin(velocity.y*10.0f);
    }
}

void Balloon::OnPurge(Level* _level){
    if(balloon_type == BalloonType::REGULAR) _level->NewActor(std::make_unique<BalloonPop>(position, olc::DARK_RED));
    if(balloon_type == BalloonType::HAZARDOUS) _level->NewActor(std::make_unique<BalloonPop>(position, olc::DARK_MAGENTA, 150));
    if(balloon_type == BalloonType::HEALTH){
        _level->NewActor(std::make_unique<BalloonPop>(position, olc::DARK_GREEN));
        _level->NewActor(std::make_unique<HeartBubble>(position));
    }
}

std::unique_ptr<Balloon> Balloon::Load(Json& _json, int _balloon_type){
    return std::make_unique<Balloon>(olc::vf2d((float)_json.GetAsInt("x"),(float)_json.GetAsInt("y")), _balloon_type);
}