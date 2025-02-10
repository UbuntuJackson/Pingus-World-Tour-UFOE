#ifndef ANIMATION
#define ANIMATION

#include <string>
#include <memory>
#include "../actor/actor.h"
#include "animated_sprite_reference.h"
#include "../external/olcPixelGameEngine.h"
class Camera;
class Json;

class Animation : public Actor{
public:
    AnimatedSpriteReference* current_animation_state;
    std::map<std::string, AnimatedSpriteReference> animation_states;
    float frame_counter = 0.0f;
    float rotation = 0.0f;
    olc::vf2d scale = {1.0f, 1.0f};
    int cycle_count = 0;

    Animation(olc::vf2d _position);

    void AddAnimationState(AnimatedSpriteReference _spr);

    void SetAnimation(std::string _name);

    void OnUpdate();

    void OnDraw(Camera* _camera);

    //std::unique_ptr<Animation> Load(Json* _json);

};

#endif