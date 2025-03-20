#pragma once

#include <widget_sprite_reference.h>
#include <ufo_maths.h>
#include <timer.h>
#include <memory>
#include "pingus_level.h"
/// @spawn;
class PingusSplashScreen : public WidgetSpriteReference{
public:
    Timer duration_timer;

    PingusSplashScreen(Vector2f _local_position) : WidgetSpriteReference(
        "pingus_splash_screen",
        Vector2f(0.0f,0.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(680.0f,480.0f),
        Vector2f(1.0f,1.0f),
        0.0f,
        0)
    {
        duration_timer.Start(6000.0f);
    }

    void OnLevelEnter(Level* _level){
        auto level = dynamic_cast<PingusLevel*>(_level);

        level->is_menu = true;
        WidgetSpriteReference::OnLevelEnter(_level);
    }

    void OnUpdate(){
        if(duration_timer.GetTimeLeft() <= 2000.0f) current_frame_index = 1.0f;
        if(duration_timer.GetTimeLeft() <= 0.0f || SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed){
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), "../res/map/title_screen/title_screen.json");
        }
    }

};