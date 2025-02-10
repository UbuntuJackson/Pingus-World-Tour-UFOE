#pragma once

#include "../level/level.h"
#include "../asset_manager/asset_manager.h"
#include "../keyboard/single_keyboard.h"
#include "../widgets/label.h"
#include "../ufo_maths/ufo_maths.h"
#include "../random/random_number_generator.h"

using namespace ufoMaths;

class TuxMathLevel : public Level{
public:
    Label* answer_label_handle = nullptr;

    void OnResourceLoad(){

        AssetManager::Get().LoadDecal("../res/tux_math_on_steroids/blue_comet.png", "blue_comet");
        AssetManager::Get().LoadDecal("../res/tux_math_on_steroids/tuxmath_moon.png", "tuxmath_moon");

        auto u_label = std::make_unique<Label>(Vector2f(0.0f, 0.0f), Vector2f(300.0f, 300.0f), "000000");
        answer_label_handle = u_label.get();

        NewActor(std::move(u_label));

        NewActor(std::make_unique<SpriteReference>(
            "tuxmath_moon",
            Vector2f(0.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(340.0f, 240.0f),
            Vector2f(1.0f, 1.0f),
            0,
            0
        ));

    }

    std::string GetInput(){
        if(SingleKeyboard::Get().GetKey(olc::NP0).is_pressed) return "0";
        if(SingleKeyboard::Get().GetKey(olc::NP1).is_pressed) return "1";
        if(SingleKeyboard::Get().GetKey(olc::NP2).is_pressed) return "2";
        if(SingleKeyboard::Get().GetKey(olc::NP3).is_pressed) return "3";
        if(SingleKeyboard::Get().GetKey(olc::NP4).is_pressed) return "4";
        if(SingleKeyboard::Get().GetKey(olc::NP5).is_pressed) return "5";
        if(SingleKeyboard::Get().GetKey(olc::NP6).is_pressed) return "6";
        if(SingleKeyboard::Get().GetKey(olc::NP7).is_pressed) return "7";
        if(SingleKeyboard::Get().GetKey(olc::NP8).is_pressed) return "8";
        if(SingleKeyboard::Get().GetKey(olc::NP9).is_pressed) return "9";
    }

    void OnUpdate(){
        Level::OnUpdate();

        int comet_type = RandomNumberGenerator::Get().RandomInt(0, 3);

        switch(comet_type){
            case 0:
            {
                
            }
            break;

            case 1:
            {

            }
            break;

            case 2:
            {

            }
            break;
        };

    }

};