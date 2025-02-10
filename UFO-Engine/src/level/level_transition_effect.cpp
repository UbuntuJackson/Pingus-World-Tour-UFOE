#include "level_transition_effect.h"
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcPixelGameEngine.h"
#include <cmath>
#include "../console/console.h"

LevelTransitionEffect::LevelTransitionEffect(std::string _path) :
Actor(olc::vf2d(0.0f, 0.0f)),
path{_path}{
    olc::vf2d window_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();
    sprite = std::make_unique<olc::Sprite>(window_size.x, window_size.y);
    decal = std::make_unique<olc::Decal>(sprite.get());

    for(int y = 0; y < decal->sprite->Size().y; y++){
        for(int x = 0; x < decal->sprite->Size().x; x++){
            decal->sprite->SetPixel(x,y,olc::BLACK);
        }
    }

    radius = window_size.x/2.0f;

    Console::Out("radius",radius);
}

bool LevelTransitionEffect::GetFinished(){
    if(radius < 0.0f){
        return true;
    }
    return false;
}
void LevelTransitionEffect::OnUpdate(){
    radius -= radius_change_rate * Engine::Get().GetDeltaTime();
    if(GetFinished()){
        radius = 0.0f;
        Engine::Get().GoToLevel(path);
    }
}
void LevelTransitionEffect::OnWidgetDraw(){
    if(start) return;

    olc::vf2d window_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();

    olc::vi2d middle_point = {(int)window_size.x/2, (int)window_size.y/2};

    float radius_change_rate_with_dt = radius_change_rate * Engine::Get().GetDeltaTime() + 2.0f;

    for(int i = middle_point.y - (int)radius-radius_change_rate_with_dt; i < middle_point.y+(int)radius+radius_change_rate_with_dt; i++){
        for(int j = middle_point.x - (int)radius-radius_change_rate_with_dt; j < middle_point.x+(int)radius+radius_change_rate_with_dt; j++){
            sprite->SetPixel(j,i,olc::BLACK);
            if((std::sqrt((j-middle_point.x)*(j-middle_point.x)+(middle_point.y-i)*(middle_point.y-i)) < radius)) sprite->SetPixel(j,i,olc::Pixel(0.0f, 0.0f, 0.0f, 0.0f));
        }   
    }
    decal->Update();
    Engine::Get().pixel_game_engine.DrawDecal(olc::vf2d(0.0f, 0.0f), decal.get());
}
