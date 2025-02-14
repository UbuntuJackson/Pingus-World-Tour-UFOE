#pragma once
#include <widget.h>
#include <ufo_maths.h>
#include <button.h>
#include <ufo_engine.h>
#include <theme.h>
#include "pingus_level.h"
class ResultScreen : public Widget{
public:
    int rank = Ranks::C;

    Button* try_again = nullptr;
    Button* back_to_level_select = nullptr;
    enum Ranks{
        S,
        A,
        B,
        C,
        D
    };

    ResultScreen(Vector2f _local_position) : Widget(_local_position, Vector2f(200.0f, 200.0f)){

    }

    void OnLevelEnter(Level* _level){
        Widget::OnLevelEnter(_level);

        auto level = dynamic_cast<PingusLevel*>(_level);

        Console::Out("Honey coin hud frame index:",level->honey_coin_hud->current_frame_index);
        Console::Out("required pingus", level->required_pingus);
        Console::Out("maxumum rescuable_pingus", level->maximum_rescuable_pingus);

        if(level->rescued_pingus >= level->required_pingus) rank = Ranks::B;
        if(level->rescued_pingus >= level->maximum_rescuable_pingus) rank = Ranks::A;
        
        if(level->honey_coin_hud->current_frame_index == 0.0f && rank <= Ranks::B) rank--;

        theme = Theme("pwt_widget_theme", 3,4,3,4);

        try_again = AddChild<Button>(Vector2f(50.0f,200.0f-65.0f),Vector2f(100.0f,20.0f),"Try again");

        back_to_level_select = AddChild<Button>(Vector2f(50.0f,200.0f-40.0f),Vector2f(100.0f,20.0f),"Back to level select");

        try_again->theme = Theme("pwt_widget_theme", 3,4,3,4);
        back_to_level_select->theme = Theme("pwt_widget_theme", 3,4,3,4);

        Console::Out("Try again rectangle",try_again->local_position);
        Console::Out("Back to level select rectangle",back_to_level_select->local_position);

        auto rank_icon = AddChild<WidgetSpriteReference>(
            "ranks",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(120.0f,120.0f),
            Vector2f(0.5f,0.5f),
            0.0f,
            0
        );

        rank_icon->current_frame_index = rank;
    }

    void OnUpdate(){
        Widget::OnUpdate();

        if(back_to_level_select->IsPressed()){
            Console::Out("Back to level select");
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(),"../res/map/title_screen/title_screen.json");
            return;
        }

        if(try_again->IsPressed()){
            Console::Out("Pressed Try again", Engine::Get().current_level->path);
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(),Engine::Get().current_level->path);
            return;
        }

    }

    void OnWidgetDraw(){
        Widget::OnWidgetDraw();

        std::string s_rank;
        std::string message;

        switch(rank){
            case Ranks::C:{
                s_rank = "C";
                message = "You failed. Too many pingu lives were lost.";
            }
            break;

            case Ranks::B:{
                s_rank = "B";
                message = "Well done, some lives were lost, but worth the sacrifice.";
            }
            break;

            case Ranks::A:{
                s_rank = "A";
                message = "Luckily no lives were lost!";
            }
            break;

            case Ranks::S:{
                s_rank = "S";
                message = "You got that sweet-sweet honeycoin. You're about to conquer the world!";
            }
            break;
        }

        std::string result_screen_text = "Rank: "+s_rank+"\n\n" + message;

        WrappedText wrapped_result_screen_text = GetWrappedTextWrapOnSpace(result_screen_text);

        Engine::Get().pixel_game_engine.DrawStringDecal(GetGlobalPosition() + Vector2f(20.0f,try_again->local_position.y-8.0f*wrapped_result_screen_text.rows-8.0f), wrapped_result_screen_text.text);
    }
};