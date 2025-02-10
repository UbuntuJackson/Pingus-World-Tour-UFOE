#ifndef COIN_SPARKLE
#define COIN_SPARKLE

#include <memory>
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "../level/actor.h"
#include "../sprite_reference/sprite_reference.h"
#include "../random/random_number_generator.h"
#include "../console/console.h"

class Sparkle : public Actor{
public:
    olc::vf2d velocity;
    Timer life_time;
    std::string sprite_name;
    Sparkle(olc::vf2d _position, olc::vf2d _velocity, std::string _sprite_name) : Actor(_position), velocity{_velocity}, sprite_name{_sprite_name}{
        life_time.Start(500.0f);
    }
    void OnLevelEnter(Level* _level){
        Console::Out("Sparkle",in_level_tree);
        auto spr = std::make_unique<SpriteReference>(sprite_name, olc::vf2d(0.0f, 0.0f), olc::vf2d(0.0f, 0.0f),olc::vf2d(5.0f,7.0f),olc::vf2d(1.0f, 1.0f),0.0f,0);
        spr->current_frame_index = RandomNumberGenerator::Get().RandomInt(0,2);
        AddChild(std::move(spr));
    }
    void OnUpdate(){
        if(life_time.GetTimeLeft() < 0.0f) Engine::Get().current_level->QueueForPurge(id);
        local_position += velocity;
    }
};

#endif