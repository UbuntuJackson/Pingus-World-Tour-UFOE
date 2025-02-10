#ifndef LEVEL_TRANSITION_EFFECT
#define LEVEL_TRANSITION_EFFECT

#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include <cmath>
#include "../console/console.h"

class LevelTransitionEffect : public Actor{
public:
    std::string path;
    float radius;
    float radius_change_rate = 210.0f;
    bool start = false;
    std::unique_ptr<olc::Sprite> sprite;
    std::unique_ptr<olc::Decal> decal;

    LevelTransitionEffect(std::string _path);

    virtual bool GetFinished();
    void OnUpdate();
    void OnWidgetDraw();
};

#endif