#include <string>
#include <memory>
#include "animation.h"
#include "../json/json.h"
#include "../camera/camera.h"
#include "../actor/actor.h"
#include "animated_sprite_reference.h"
#include "../external/olcPixelGameEngine.h"
#include "../ufo_engine/ufo_engine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../drawing_system/drawing_system.h"

Animation::Animation(olc::vf2d _position) : Actor(_position){}

void Animation::AddAnimationState(AnimatedSpriteReference _spr){
    animation_states.emplace(_spr.key, std::move(_spr));
}

void Animation::SetAnimation(std::string _name){
    current_animation_state = &animation_states.at(_name);
    cycle_count = 0;
    //frame_counter = 0.0f;
}

void Animation::OnUpdate(){
    current_animation_state->position = position;
    frame_counter += Engine::Get().GetDeltaTime() * current_animation_state->pace;
    if(frame_counter >= current_animation_state->number_of_frames) cycle_count++;
    frame_counter = ufoMaths::Wrap(frame_counter, 0.0f, (float)current_animation_state->number_of_frames);
    current_animation_state->current_frame_index = (int)frame_counter;
}

void Animation::OnDraw(Camera* _camera){
    current_animation_state->position = position;
    DrawingSystem::Draw(*current_animation_state, _camera);
}

//std::unique_ptr<Animation> Animation::Load(Json* _json){}