#include <memory>
#include <cmath>
#include "../external/olcPixelGameEngine.h"
#include "../sprite_reference/sprite_reference.h"
#include "../level/actor.h"
#include "../level/level.h"
#include "../ufo_clock/timer.h"
#include "../ufo_maths/ufo_maths.h"
#include "../console/console.h"
#include "tongue.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"
#include "frogatto.h"
#include "frogatto_level.h"
#include "ant.h"
#include "../shapes/circle.h"
#include "../shapes/shape.h"
#include "../drawing_system/drawing_system.h"
#include "edible.h"

Tongue::Tongue(olc::vf2d _position, Frogatto* _frogatto, olc::vf2d _direction) :
    Actor(_position),
    frogatto{_frogatto},
    direction{_direction}
{

}

void Tongue::OnLevelEnter(Level* _level){
    level = dynamic_cast<FrogattoLevel*>(_level);

    life_time_timer.Start(500.0f);

    auto u_sprite_reference = std::make_unique<SpriteReference>(
        "tongue",
        olc::vf2d(0.0f, 0.0f),
        olc::vf2d(0.0f, 0.0f),
        olc::vf2d(16.0f, 16.0f),
        olc::vf2d(1.0f, 1.0f),
        0.0f, 0, 0.0f
    );

    sprite_reference = u_sprite_reference.get();

    sprite_reference->scale.x = direction.x;
    if(direction.y < 0.0f && direction.x == 0.0f){
        sprite_reference->rotation = -ufoMaths::PI/2.0f;
        sprite_reference->scale.x = 1.0f;
        sprite_reference->local_position.x = -8.0f+-4*direction.x;
        sprite_reference->local_position.y = 8.0f;
    }
    if(direction.y < 0.0f && std::abs(direction.x) > 0.0f){
        sprite_reference->scale.x = direction.x;
        sprite_reference->rotation = -ufoMaths::PI/4.0f * direction.x;
        sprite_reference->local_position.x = -4.0f*direction.x;
        sprite_reference->local_position.y = 0.0f;
    }

    AddChild(std::move(u_sprite_reference));

    auto u_hitbox = std::make_unique<Shape<Circle>>(Circle(olc::vf2d(0.0f, 0.0f), 6.0f),olc::vf2d(0.0f, 0.0f));

    hitbox = u_hitbox.get();

    AddChild(std::move(u_hitbox));
    hitbox->local_position.x = sprite_reference->frame_size.x*sprite_reference->scale.x;
    hitbox->local_position.y = 8.0f;
}

void Tongue::OnUpdate(){

    for(auto&& edible : level->edible_handles){
        
        if(edible->CheckOverlap(&(hitbox->shape))){
            edible->OnEaten(frogatto);
            edible->QueueForPurge();
            extended_phase_finished = true;
        }
    }

    /*for(auto&& enemy : level->enemy_handles){
        if(enemy->shape->IsOverlappingOther(&(hitbox->shape))){
            enemy->OnEaten(frogatto);
            enemy->QueueForPurge();
            extended_phase_finished = true;
        }
    }*/

    for(const auto& balloon : level->balloon_handles){
        if(balloon->shape.IsOverlappingOther(&(hitbox->shape))){
            balloon->QueueForPurge();
            if(balloon->balloon_type == BalloonType::HAZARDOUS) frogatto->TakeDamage();
        }
    }

    for(const auto& heart_bubble : level->heart_bubble_handles){
        if(heart_bubble->shape->IsOverlappingOther(&(hitbox->shape))){
            if(heart_bubble->invincibility_timer.GetTimeLeft() < 0.0f){
                frogatto->health++;
                heart_bubble->QueueForPurge();
                for(int i = 0; i < 3; i++){
                float r_number = RandomNumberGenerator::Get().RandomFloat(-8.0f,8.0f);
                    level->NewActor(
                        std::make_unique<Sparkle>(
                            olc::vf2d(heart_bubble->position.x+r_number, heart_bubble->position.y-r_number*1.5f), olc::vf2d(0.0f, -1.0f), "heart_sparkle"));
                }
            }
        }
    }

    if(std::abs(sprite_reference->scale.x * sprite_reference->frame_size.x) < tongue_max_size && !extended_phase_finished){
        sprite_reference->scale.x *= 1.1f;
    }
    else{
        extended_phase_finished = true;
        sprite_reference->scale.x /= 1.5f;
    }

    Console::Out(direction.x);
    if(direction.y == 0.0f) hitbox->local_position.x = sprite_reference->frame_size.x*sprite_reference->scale.x;

    if(direction.y < 0.0f){
        hitbox->local_position.x = 0.0f;
        hitbox->local_position.y = std::abs(sprite_reference->frame_size.x*sprite_reference->scale.x) * direction.y + 8.0f;
    }

    if(direction.y < 0.0f && std::abs(direction.x) > 0.0f){
        hitbox->local_position.x = sprite_reference->frame_size.x*sprite_reference->scale.x;
        hitbox->local_position.y = std::abs(sprite_reference->frame_size.x*sprite_reference->scale.x) * direction.y + 8.0f;
    }

    for(const auto& line : level->collision_system.lines){
        CollisionData result = ufoMaths::RayVsRay(Ray2(position+olc::vf2d(0.0f, 8.0f), position+olc::vf2d(0.0f, 8.0f) + direction * sprite_reference->scale.x * sprite_reference->frame_size.x),line);
        if(result.is_hit) Console::Out("Tongue hit terrain");

    }

    if(life_time_timer.GetTimeLeft() <= 0.0f){
        QueueForPurge();
    }


}

void Tongue::OnDraw(Camera* _camera){
    DrawingSystem::Draw(Ray2(position+olc::vf2d(0.0f, 8.0f), position+olc::vf2d(0.0f, 8.0f) + direction * std::abs(sprite_reference->scale.x) * sprite_reference->frame_size.x), _camera, olc::BLUE);
}