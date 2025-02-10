#include "../level/actor.h"
#include "../level/level.h"
#include "../ufo_maths/ufo_maths.h"
#include "../sprite_reference/sprite_reference.h"
#include "../ufo_clock/timer.h"
#include "dying_sequence.h"
#include "../camera/camera.h"
#include "../level/ufo_engine.h"
#include "tuxtale_player.h"

DyingSequence::DyingSequence(Vector2f _local_position) : Actor(_local_position){

}

void DyingSequence::OnLevelEnter(Level *_level){
    Actor::OnLevelEnter(_level);

    level = _level;

    auto death_sprite = AddChild<SpriteReference>(
        "tux_dying",
        Vector2f(0.0f, 0.0f),
        Vector2f(10.0f, 8.0f),
        Vector2f(20.0f, 16.0f),
        Vector2f(1.0f, 1.0f),
        0.0f,
        0
    );

    death_sprite_id = death_sprite->id;

    AddChild<Camera>(Vector2f(0.0f, 0.0f));

    timer.Start(700.0f);
}

void DyingSequence::OnUpdate(){
    Actor::OnUpdate();

    if(timer.GetTimeLeft() <= 0.0f){
        level->QueueForPurge(death_sprite_id);

        Console::Out("Spawn point in direction:",(level->spawn_point-GetGlobalPosition()).norm());
        Vector2f dir = (level->spawn_point-GetGlobalPosition()).norm();

        float former_dist = (level->spawn_point-GetGlobalPosition()).mag();
        
        velocity += dir * 1000.0f * Engine::Get().GetDeltaTime();
        local_position += velocity * Engine::Get().GetDeltaTime();

        float current_dist = (level->spawn_point-GetGlobalPosition()).mag();

        if(former_dist < current_dist){
            local_position = level->spawn_point;
            level->NewActor<TuxTalePlayer>(level->spawn_point);
            QueueForPurge();
        }
    }
}