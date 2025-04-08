#pragma once

#include <vector>
#include <string>
#include <actor.h>
#include <level.h>
#include <widget.h>
#include <memory>
#include <ufo_maths.h>
#include <widget_sprite_reference.h>
#include <one_dimensional_text_box.h>
#include <algorithm>
#include <single_keyboard.h>
#include <nine_patch_theme.h>
#include "pingus_level.h"
/// @spawn;
class StoryModeIntro : public Actor{
public:

    OneDimensionalTextBox* text_box = nullptr;

    WidgetSpriteReference* image = nullptr;

    int text_box_counter = 0;

    StoryModeIntro(Vector2f _local_position) : Actor(Vector2f(0.0f,0.0f)){

    }

    void OnLevelEnter(Level* _level){
        auto u_text_box = std::make_unique<OneDimensionalTextBox>(Vector2f(10.0f, 200.0f),Vector2f(400.0f, 50.0f));

        u_text_box->text_queue.push_back("These are the wild pingus in their natural habitat.");
        u_text_box->text_queue.push_back("The pingus are small, penguin-like creatures found on the shores of Antarctica. Where they live in harmony in large groups.");
        u_text_box->text_queue.push_back("Or so they used to...");
        
        u_text_box->text_queue.push_back("One day things started to change. The climate got hotter. The icesheets melted away. Chaos ensued.");
        u_text_box->text_queue.push_back("The pingus assembled a team to set out into the world to find out what was happening.");
        u_text_box->text_queue.push_back("They are off to a good start, but something is missing. They are prone to walk off ledges");
        u_text_box->text_queue.push_back("What they need is leadership, and it is up to YOU, the player to guide them on their WORLD TOUR.");
        u_text_box->text_queue.push_back("~ The Pingus' World Tour ~\n(c) UFO-Engine Studios 2025");

        std::reverse(u_text_box->text_queue.begin(), u_text_box->text_queue.end());

        text_box = u_text_box.get();

        _level->NewActor(std::move(u_text_box));

        image = AddChild<WidgetSpriteReference>(
            "pwt_intro_cutscene",
            Vector2f(0.0f,0.0f),Vector2f(0.0f,0.0f),
            Vector2f(680.0f,480.0f),Vector2f(1.0f,1.0f),
            0.0f,
            0
        );

        dynamic_cast<PingusLevel*>(_level)->is_menu = true;

        Vector2f window_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();
        text_box->local_position.x = window_size.x/2 - text_box->GetRectangle().size.x/2;
        text_box->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        text_box->text_wrapping_mode = Widget::TextWrappingModes::WORD_MEETS_BORDER;

    }

    void OnUpdate(){

        if(text_box){
            if(SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed){
                bool has_next = text_box->Next();
                if(!has_next){
                    text_box = nullptr;

                    Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), "../res/map/world_map/world_map.json");
                }
                text_box_counter++;

                if(text_box_counter == 3){
                    text_box->local_position.y -= 50.0f;
                    image->key = "pwt_intro_cutscene3";

                }

                if(text_box_counter == 4){
                    text_box->local_position.y = 20.0f;

                    image->key = "cover_for_intro";
                }
            }
        }

    }

};