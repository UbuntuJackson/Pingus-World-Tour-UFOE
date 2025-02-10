#ifndef SPLASH_SCREEN
#define SPLASH_SCREEN
#include <string>
#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "level.h"
#include "../asset_manager/asset_manager.h"
#include "../ufo_clock/timer.h"
#include "ufo_engine.h"
#include "../sprite_reference/widget_sprite_reference.h"
#include "../console/console.h"
#include "../random/random_number_generator.h"
#include "../keyboard/single_keyboard.h"

class SplashScreen : public Level{
public:
    Timer display_time;
    SpriteReference* fading_logo;
    std::unique_ptr<Level> queued_level = nullptr;
    std::string next_level_path;
    int r;
    int g;
    int b;
    SplashScreen(std::string _next_level_path) : Level(),next_level_path{_next_level_path}{
        display_time.Start(1000.0f);
        r = RandomNumberGenerator::Get().RandomInt(1,60);
        g = RandomNumberGenerator::Get().RandomInt(1,60);
        b = RandomNumberGenerator::Get().RandomInt(1,60);
    }
    SplashScreen(std::unique_ptr<Level> _next_level, std::string _next_level_path) : Level(),next_level_path{_next_level_path}{
        queued_level = std::move(_next_level);
        
        display_time.Start(1000.0f);
        r = RandomNumberGenerator::Get().RandomInt(1,60);
        g = RandomNumberGenerator::Get().RandomInt(1,60);
        b = RandomNumberGenerator::Get().RandomInt(1,60);
    }

    void OnResourceLoad(){
        AssetManager::Get().LoadDecal("../res/engine/ufo_engine_transparent_logo.png","splash_screen");
        
        olc::vf2d window_size_pixels = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();

        olc::vf2d scale = olc::vf2d(4.0f,4.0f)/olc::vf2d(Engine::Get().pixel_game_engine.GetPixelSize().x, Engine::Get().pixel_game_engine.GetPixelSize().y);

        auto u_fading_logo = std::make_unique<WidgetSpriteReference>(
            "splash_screen",
            olc::vf2d(window_size_pixels.x/2.0f-(217.0f/2.0f)*scale.x, window_size_pixels.y/2.0f-(240.0f/2.0f)*scale.y),
            //olc::vf2d(window_size_pixels.x/2.0f-(217.0f/2.0f)*4/Engine::Get().pixel_game_engine.GetPixelSize().x, window_size_pixels.y/2.0f-(240.0f/2.0f)*4/Engine::Get().pixel_game_engine.GetPixelSize().y),
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(217.0f, 240.0f),
            scale,
            0.0f, 0);

        fading_logo = u_fading_logo.get();

        NewActor(std::move(u_fading_logo));
    }
    void OnUpdate(){

        Level::OnUpdate();
        
        
        Engine::Get().pixel_game_engine.FillRectDecal(olc::vi2d(0, 0), Engine::Get().pixel_game_engine.GetWindowSizeInPixles(), olc::Pixel(r,g,b));
        

        if(display_time.GetTimeLeft() <= 0.0f){
            if(!queued_level.get()) Engine::Get().GoToLevel(next_level_path);
            else Engine::Get().GoToLevel(std::move(queued_level), next_level_path);
        }
    }
};

#endif