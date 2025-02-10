#include <string>
#include <utility>
#include <unordered_map>
#include "animation_bundle.h"
#include "../external/olcPixelGameEngine.h"
#include "../console/console.h"
#include "../json/json.h"
#include "../asset_manager/asset_manager.h"
#include "../asset_manager/get_rect_from_index.h"
#include "../ufo_clock/ufo_time.h"
#include "../ufo_maths/ufo_maths.h"
#include "../camera/camera.h"
#include "../graphics_provider/graphics_provider.h"
#include "animation_manager.h"
#include "../scene/scene.h"

Animation::Animation(
    int _number_of_frames,
    float _frame_duration
):
number_of_frames{_number_of_frames},
frame_duration{_frame_duration},
frame_duration_factor{1.0f},
parent_frame_duration_factor{&frame_duration_factor}
{}

AnimationBundle::AnimationBundle(std::string _asset_folder, std::string _file_name, std::string _initial_state) :
initial_state{_initial_state}
{
    Json j = Json::Read(_asset_folder+_file_name);
    Json::ArrayForEach(Json(), j.GetObject("animations"), [&](Json _item, Json _array){
        std::string name = _item.GetAsString("name");
        std::string path = _asset_folder + _item.GetAsString("path");
        int number_of_frames = _item.GetAsInt("number-of-frames");
        size = {float(_item.GetAsInt("width")), float(_item.GetAsInt("height"))};
        offset = {float(_item.GetAsInt("offset-x")), float(_item.GetAsInt("offset-y"))};
        float frame_duration = (float)_item.GetAsDouble("frame-duration");
        AssetManager::Get().LoadDecal(path, name);
        animation_states.emplace(name, Animation(number_of_frames, frame_duration));
    }
    );
}

AnimationBundle::~AnimationBundle(){
    for(const auto& [k, animation_state] : animation_states){
        AssetManager::Get().RemoveAsset(k);
        Console::Out("[!]","Assets for animation bundle with name", k, "were removed.");
    }
}

AnimationBundleReference::AnimationBundleReference(std::string _name, olc::vf2d _position) :
name{_name},
position{_position},
parent_position{&position},
frame_progress_counter{0.0f}
{
    current_state = AnimationManager::Get().GetAnimationBundle(_name)->initial_state;
    parent_state = &current_state;
}

void AnimationBundleReference::Update(Scene* _scene){
    position = *parent_position;
    UpdateState(*parent_state);
    DrawAnimation(this, _scene->GetActiveCamera());
}

void AnimationBundleReference::SetState(std::string _state_name){
    auto animation_bundle = AnimationManager::Get().GetAnimationBundle(name);
    auto animation_states = animation_bundle->animation_states;
    if(animation_states.size() == 0) return;
    if(!animation_states.count(_state_name)){
        Console::Out("[!]", "[Filmtape::UpdateState()]", "State was not found");
        return;
    }
    if(_state_name != current_state){
        frame_progress_counter = 0.0f;
        Console::Out("Frame progress counter reset");
    }
    current_state = _state_name;

    auto animation = animation_states.at(current_state);
    animation.frame_duration_factor = *(animation.parent_frame_duration_factor);
}

std::pair<std::string, Animation> AnimationBundleReference::GetCurrentAnimationData(){
    auto animation_states = AnimationManager::Get().GetAnimationBundle(name)->animation_states;
    return std::make_pair(current_state,animation_states.at(current_state));
}

void AnimationBundleReference::UpdateState(std::string _state_name){
    SetState(_state_name);
    AnimationBundle *animation_bundle = AnimationManager::Get().GetAnimationBundle(name);
    auto animation = animation_bundle->animation_states.at(current_state);

    float number_of_frames = (float)animation_bundle->animation_states.at(current_state).number_of_frames;

    frame_progress_counter += animation.frame_duration * animation.frame_duration_factor * ufoTime::GetDeltaTime();
    frame_progress_counter = ufoMaths::Wrap(frame_progress_counter, 0.0f, (float)number_of_frames);
}

void DrawAnimation(AnimationBundleReference* _animation_bundle_reference, Camera* _camera){
    AnimationBundle *animation_bundle = AnimationManager::Get().GetAnimationBundle(_animation_bundle_reference->name);

    auto animation_data = _animation_bundle_reference->GetCurrentAnimationData();
    auto decal = AssetManager::Get().GetDecal(animation_data.first);
    auto frame_size = animation_bundle->size;
    Rectangle sample_rectangle = GetFrameFromSpriteSheet(animation_data.first,_animation_bundle_reference->frame_progress_counter,frame_size);
    float scale = _camera->scale;

    GraphicsProvider::Get().DrawPartialRotatedDecal(
        _camera->Transform(_animation_bundle_reference->position+animation_bundle->offset),
        decal,
        0.0f,
        {0.0f, 0.0f},
        sample_rectangle.position,
        sample_rectangle.size,
        {scale, scale}
    );
}