#ifndef SPRITE_SHEET_ANIMATION_H
#define SPRITE_SHEET_ANIMATION_H
#include <string>
#include <utility>
#include <unordered_map>
#include "../external/olcPixelGameEngine.h"
#include "../console/console.h"
#include "../json/json.h"
#include "../asset_manager/asset_manager.h"

class Camera;
class Scene;

struct Animation{
    int number_of_frames;
    float frame_duration;
    float frame_duration_factor;
    float *parent_frame_duration_factor;
    Animation(
        int _number_of_frames,
        float _frame_duration
    );
};

class AnimationBundle{
public:
    olc::vf2d offset;
    olc::vf2d size;

    //                        name
    std::unordered_map<std::string, Animation> animation_states;
    std::string initial_state;

    AnimationBundle(std::string _asset_folder, std::string _file_name, std::string _initial_state);

    ~AnimationBundle();
};

struct AnimationBundleReference{
    olc::vf2d position;
    olc::vf2d* parent_position;
    std::string name;
    std::string *parent_state;
    std::string current_state;
    float frame_progress_counter;

    AnimationBundleReference(std::string _name, olc::vf2d _position);
    std::pair<std::string, Animation> GetCurrentAnimationData();
    void Update(Scene* _scene);
    void SetState(std::string _name);
    void UpdateState(std::string _state_name);
};

void DrawAnimation(AnimationBundleReference* _film_tape, Camera* _camera);

#endif