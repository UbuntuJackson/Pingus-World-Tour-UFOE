#pragma once
#include <widget.h>
#include <ufo_maths.h>
#include <button.h>
#include <ufo_engine.h>
#include <theme.h>
#include <nine_patch_theme.h>
#include "pingus_level.h"
class ResultScreen : public Widget{
public:
    int rank = Ranks::C;

    Button* try_again = nullptr;
    Button* back_to_level_select = nullptr;
    Button* back_to_world_map = nullptr;
    enum Ranks{
        S,
        A,
        B,
        C,
        D
    };

    ResultScreen(Vector2f _local_position) : Widget(_local_position, Vector2f(200.0f, 250.0f)){

    }

    void OnLevelEnter(Level* _level){
        Widget::OnLevelEnter(_level);

        auto level = dynamic_cast<PingusLevel*>(_level);

        if(level->rescued_pingus >= level->required_pingus) rank = Ranks::B;
        if(level->rescued_pingus >= level->maximum_rescuable_pingus) rank = Ranks::A;
        
        if(level->honey_coin_hud->current_frame_index == 0.0f && rank <= Ranks::B) rank--;

        theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);

        try_again = AddChild<Button>(Vector2f(50.0f,200.0f-65.0f),Vector2f(100.0f,20.0f),"Try again");

        back_to_world_map = AddChild<Button>(Vector2f(50.0f,200.0f-40.0f),Vector2f(100.0f,20.0f),"Back to World Map");

        back_to_level_select = AddChild<Button>(Vector2f(50.0f,200.0f-7.0f),Vector2f(100.0f,20.0f),"Back to level select");

        //Set the themes
        try_again->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        try_again->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
        try_again->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);

        back_to_world_map->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        back_to_world_map->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
        back_to_world_map->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);

        back_to_level_select->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        back_to_level_select->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
        back_to_level_select->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);

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
        JsonVariant variant_cleared_levels = Engine::Get().GetActiveProfile()->save_file.Get("cleared_levels");
        if(!variant_cleared_levels.IsNull()){
            JsonDictionary& cleared_levels = variant_cleared_levels.AsDictionary();

            JsonVariant& previous_level_high_score_data = cleared_levels.Get(_level->path);

            if(previous_level_high_score_data.IsNull()){

                JsonDictionary level_high_score_data = JsonDictionary();

                level_high_score_data.Set("rank", rank);
                
                level_high_score_data.Set("most_rescued_pingus", level->rescued_pingus);

                cleared_levels.Set(_level->path, level_high_score_data);
            }
            else{

                int last_rank = previous_level_high_score_data.AsDictionary().Get("rank").AsInt();
                if(rank < last_rank) previous_level_high_score_data.AsDictionary().Set("rank", rank);

                int rescued_pingus_high_score = previous_level_high_score_data.AsDictionary().Get("most_rescued_pingus").AsInt();
                if(rescued_pingus_high_score < level->released_pingus) previous_level_high_score_data.AsDictionary().Set("most_rescued_pingus", level->rescued_pingus);
            }

            Engine::Get().GetActiveProfile()->save_file.Set("last_played_level", _level->path);

            Engine::Get().GetActiveProfile()->Save();
        }
        Vector2f window_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();
        local_position = window_size/2 - GetRectangle().size/2;
    }

    void OnPaused(){
        //Widget::OnUpdate();

        if(back_to_level_select->IsReleased()){
            Console::Out("Back to level select");
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(),"../res/map/title_screen/title_screen.json");
            return;
        }

        if(back_to_world_map->IsReleased()){
            
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(),"../res/map/world_map/world_map.json");
            return;
        }

        if(try_again->IsReleased()){
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
                s_rank = "L for Loss.";
                message = "But don't feel down! Try better next time!";
            }
            break;

            case Ranks::B:{
                s_rank = "B for Brilliant!";
                message = "Some friends were left behind, but course was cleared.";
            }
            break;

            case Ranks::A:{
                s_rank = "A for Absolutely incredible!";
                message = "Luckily no lives were lost!";
            }
            break;

            case Ranks::S:{
                s_rank = "S for Superb!";
                message = "You got that sweet-sweet honeycoin!";
            }
            break;
        }

        std::string result_screen_text = "Rank: "+s_rank+"\n" + message;

        WrappedText wrapped_result_screen_text = GetWrappedTextWrapOnSpace(result_screen_text);

        Engine::Get().pixel_game_engine.DrawStringDecal(GetGlobalPosition() + Vector2f(20.0f,try_again->local_position.y-8.0f*wrapped_result_screen_text.rows-8.0f), wrapped_result_screen_text.text);
    }
};