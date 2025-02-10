#ifndef FROGATTO
#define FROGATTO

#include <memory>
#include <functional>
#include "../external/olcPixelGameEngine.h"
#include "../level/player_body.h"
#include "../json/json.h"
#include "../level/ufo_engine.h"
#include "../ufo_maths/ufo_maths.h"
#include "balloon.h"
#include "frogatto_hud.h"
#include "../ufo_clock/timer.h"
#include "../console/console.h"
#include "../animation/animation.h"
#include "../animation/animated_sprite_reference.h"
#include "frogatto_level.h"
#include "../shapes/circle.h"
#include "moving_sprite.h"
#include "../random/random_number_generator.h"
#include "heart_bubble.h"
#include "../shapes/collision_circle.h"
#include "../shapes/collision_pill.h"
#include "tongue.h"
#include "ant.h"
#include "water_body.h"

class Frogatto : public PlayerBody{
public:
    bool is_in_water = false;
    bool was_in_water = false;

    FrogattoHud* hud;
    int health = 4;
    float magic = 54.0f;
    int coins = 0;
    Timer invincibility_timer;
    Animation* animation;
    FrogattoLevel* level;
    Timer attack_animation_timer;
    
    std::function<std::unique_ptr<Actor>(Frogatto* _frogatto)> eaten_actor_spawner = nullptr;
    olc::vf2d tongue_grab_location;

    Frogatto(olc::vf2d _position, FrogattoLevel* _level) : PlayerBody(_position){

        /*GROUND_ACCELERATION *= 60.0f;
        AIR_ACCELERATION *= 60.0f;

        GROUND_FRICTION *= 60.0f;
        AIR_FRICTION *= 60.0f;*/

    }

    void OnLevelEnter(Level* _level){
        children.clear(); //This probably clears the collision shape. Stupid.

        PlayerBody::OnLevelEnter(_level);

        SetShape(CollisionCircle(this,olc::vf2d(0.0f, 0.0f),7.0f),olc::vf2d(0.0f, 0.0f));
        //SetShape(CollisionPill(olc::vf2d(0.0f, 0.0f),5.0f,7.0f,this),olc::vf2d(0.0f, -5.0f));

        auto u_animation = std::make_unique<Animation>(olc::vf2d(0.0f, 0.0f));

        animation = u_animation.get();

        u_animation->AddAnimationState(AnimatedSpriteReference(
            "frogatto_idle",
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(8.0f, 8.0f),
            olc::vf2d(16.0f, 16.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0, 0.0f));

        u_animation->AddAnimationState(AnimatedSpriteReference(
            "frogatto_hurt",
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(8.0f, 8.0f),
            olc::vf2d(16.0f, 16.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0, 0.0f));
        
        u_animation->AddAnimationState(AnimatedSpriteReference(
            "frogatto_walk",
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(8.0f, 8.0f),
            olc::vf2d(16.0f, 16.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0, 7.0f));

        u_animation->AddAnimationState(
            AnimatedSpriteReference(
            "frogatto_jump",
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(8.0f, 8.0f),
            olc::vf2d(16.0f, 16.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0, 7.0f));

        u_animation->AddAnimationState(
            AnimatedSpriteReference(
            "frogatto_tongue_attack",
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(8.0f, 8.0f),
            olc::vf2d(16.0f, 16.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0, 7.0f));

        u_animation->SetAnimation("frogatto_idle");

        AddChild(std::move(u_animation));

        auto fh = std::make_unique<FrogattoHud>();
        hud = fh.get();
        _level->NewActor(std::move(fh));

        level = dynamic_cast<FrogattoLevel*>(_level);
    }

    void Accelerate(){
        was_in_water = is_in_water;
        is_in_water = false;

        for(const WaterBody* water_body : level->water_body_handles){
            if(ufoMaths::RectangleVsPoint(water_body->shape, position)){
                is_in_water = true;
            }
        }

        if(!is_in_water){
            PlayerBody::Accelerate();
        }
        else{
            olc::vf2d acceleration = olc::vf2d(0.2f,0.2f);
            float friction = 0.15f;

            float horizontal_direction = int(SingleKeyboard::Get().GetKey(olc::RIGHT).is_held) - int(SingleKeyboard::Get().GetKey(olc::LEFT).is_held);
            float vertical_direction = int(SingleKeyboard::Get().GetKey(olc::DOWN).is_held) - int(SingleKeyboard::Get().GetKey(olc::UP).is_held);

            Console::Out(horizontal_direction, vertical_direction);

            if(horizontal_direction != 0){
                face_direction = horizontal_direction;
            }
            
            acceleration.x = acceleration.x*horizontal_direction;
            acceleration.y = acceleration.y*vertical_direction;

            if(MAX_SPEED > std::abs(velocity.x)) velocity.x += acceleration.x;
            if(MAX_SPEED > std::abs(velocity.y)) velocity.y += acceleration.y;

            float hspeed_prefriction = velocity.x;
            float vspeed_prefriction = velocity.y;
            velocity.x-=friction*ufoMaths::Sign(velocity.x);
            velocity.y-=friction*ufoMaths::Sign(velocity.y);

            if(velocity.x * hspeed_prefriction < 0.0f) velocity.x = 0.0f;
            if(velocity.y * vspeed_prefriction < 0.0f) velocity.y = 0.0f;
        }

        if(was_in_water && !is_in_water){
            velocity.y = -5.0f;
        }
    }

    void OnUpdate(){
        PlayerBody::OnUpdate();

        if(health <= 0) Die();

        for(const auto& balloon : level->balloon_handles){
            if(balloon->shape.IsOverlappingOther(shape)){
                velocity.y = -5.0f;
                balloon->QueueForPurge();
                if(balloon->balloon_type == BalloonType::HAZARDOUS) TakeDamage();
            }
        }

        for(const auto& heart_bubble : level->heart_bubble_handles){
            if(heart_bubble->shape->IsOverlappingOther(shape)){
                if(heart_bubble->invincibility_timer.GetTimeLeft() < 0.0f){
                    health++;
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

        for(const auto& enemy : level->enemy_handles){
            if(enemy->shape->IsOverlappingOther(shape)){
                if(invincibility_timer.GetTimeLeft() < 0.0f) velocity.y = -2.5f;
                TakeDamage();
            }
        }

        for(const auto& coin : level->coin_handles){
            if(coin->shape.IsOverlappingOther(shape)){
                level->QueueForPurge(coin->id);
                coins += 1;
                for(int i = 0; i < 3; i++){
                    float r_number = RandomNumberGenerator::Get().RandomFloat(-8.0f,8.0f);
                    level->NewActor(
                        std::make_unique<Sparkle>(
                            olc::vf2d(coin->position.x+r_number, coin->position.y-r_number*1.5f), olc::vf2d(0.0f, -1.0f), "coin_sparkle"));
                }
            }
        }

        if(std::abs(velocity.x) > 0.01f) animation->SetAnimation("frogatto_walk");
        else animation->SetAnimation("frogatto_idle");
        if(!is_on_ground && !was_on_ground) animation->SetAnimation("frogatto_jump");
        if(invincibility_timer.GetTimeLeft() > 0.0f) animation->SetAnimation("frogatto_hurt");

        animation->current_animation_state->scale.x = face_direction;

        //Console::Out(invincibility_timer.GetTimeLeft());
        
        //Switch ability
        if(SingleKeyboard::Get().GetKey(olc::C).is_pressed){
            hud->SwitchAbility();
        }

        //magic
        switch(hud->current_ability){
            case Abilities::FIRE :{
                if(SingleKeyboard::Get().GetKey(olc::X).is_held) UseMagic();
                else magic += 0.06;
            }
            break;
            case Abilities::TONGUE :{
                
                if(SingleKeyboard::Get().GetKey(olc::X).is_pressed){
                    if(eaten_actor_spawner == nullptr){
                        magic = 0.0f;
                        float tongue_horizontal_direction = face_direction;
                        float tongue_vertical_direction = 0.0f;
                        if(SingleKeyboard::Get().GetKey(olc::UP).is_held){
                            tongue_vertical_direction = -1.0f;
                            if(SingleKeyboard::Get().GetKey(olc::RIGHT).is_held || SingleKeyboard::Get().GetKey(olc::LEFT).is_held){
                                tongue_horizontal_direction = face_direction;
                            }
                            else{
                                tongue_horizontal_direction = 0.0f;
                            }
                        }

                        AddChild(std::make_unique<Tongue>(olc::vf2d(4.0f*face_direction, -8.0f), this, olc::vf2d(tongue_horizontal_direction, tongue_vertical_direction)));
                        attack_animation_timer.Start(500.0f);
                    }
                    else{
                        level->NewActor(std::move(eaten_actor_spawner(this)));
                        eaten_actor_spawner = nullptr;
                    }
                }
                else{
                    magic+=0.1f;
                    magic*=1.1f;
                }

                if(attack_animation_timer.GetTimeLeft() >= 0.0f){
                    animation->SetAnimation("frogatto_tongue_attack");
                    animation->current_animation_state->scale.x = face_direction;
                }
                
            }
            break;
            case Abilities::RUSH :{
                if(SingleKeyboard::Get().GetKey(olc::X).is_held){
                    UseMagic();
                    if(magic > 0.0f){

                        if(velocity.mag() < MAX_SPEED*10.0f && velocity.mag() != 0.0f && (collision_normal.x != 0.0f || collision_normal.y != 0.0f)){
                            velocity += velocity.norm()*0.2;
                        }
                        Console::Out("Velocity after ability",-collision_normal * velocity.mag()*0.5f);
                    }
                }
                else{
                    magic+=0.06f;
                }
            }
            break;

        }

        if(magic > 54.0f) magic = 54.0f;
        hud->magic = int(magic);
        hud->coins = coins;
        hud->SetHealth(health);

        if(position.y > level->level_size.y+16.0f){
            Die();
        }
    }

    void Jump(float _speed){
        //Console::Out(collision_normal);
        if(SingleKeyboard::Get().GetKey(jump_button).is_pressed && (is_on_ground || was_on_ground)){
            velocity.y = _speed;
        }
    }
    void TakeDamage(){
        if(invincibility_timer.GetTimeLeft() <= 0.0f){
            health-=1;
            invincibility_timer.Start(500.0f);
        }
    }
    void UseMagic(){
        magic-=0.12f;
        if(magic < 0.0f) magic = 0.0f;
    }

    static std::unique_ptr<Frogatto> Load(Json& _json, FrogattoLevel* _level){
        float x = (float)_json.GetAsInt("x");
        float y = (float)_json.GetAsInt("y");
        return std::make_unique<Frogatto>(olc::vf2d(x, y), _level);
    }

    void Die(){Engine::Get().GoToLevel(Engine::Get().current_level->path);}

    void OnPurge(Level* _level){
        hud->QueueForPurge();
    }

    void OnDebugDraw(Camera* _camera){
        //Engine::Get().pixel_game_engine.DrawStringDecal(_camera->Transform(position), "Position: " + std::to_string(position.x)+ ", "+ std::to_string(position.y));
    }

};

#endif