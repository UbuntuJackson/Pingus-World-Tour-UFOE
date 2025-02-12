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

        if(level->rescued_pingus >= level->required_pingus) rank = Ranks::B;
        if(level->rescued_pingus == level->total_number_of_pingus) rank = Ranks::A;
        if(level->rescued_pingus == level->total_number_of_pingus && level->honey_coin_hud->current_frame_index == 0.0f) rank = Ranks::S;

        theme = Theme("pwt_widget_theme", 3,4,3,4);

        try_again = AddChild<Button>(Vector2f(50.0f,200.0f-65.0f),Vector2f(100.0f,20.0f),"Try again");

        back_to_level_select = AddChild<Button>(Vector2f(50.0f,200.0f-40.0f),Vector2f(100.0f,20.0f),"Back to level select");

        try_again->theme = Theme("pwt_widget_theme", 3,4,3,4);
        back_to_level_select->theme = Theme("pwt_widget_theme", 3,4,3,4);

        Console::Out("Try again rectangle",try_again->local_position);
        Console::Out("Back to level select rectangle",back_to_level_select->local_position);
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

        Console::Out("Try again rectangle",try_again->local_position, try_again->GetGlobalPosition(), try_again->rectangle.position);

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

        std::string result_screen_text = "Rank:" + s_rank + "\n\n" + message;

        Engine::Get().pixel_game_engine.DrawStringDecal(GetGlobalPosition() + Vector2f(20.0f,20.0f), GetWrappedTextWrapOnSpace(result_screen_text).text);
    }
};