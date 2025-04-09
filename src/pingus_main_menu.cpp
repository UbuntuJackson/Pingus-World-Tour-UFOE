#include "pingus_main_menu.h"
#include <memory>
#include <functional>
#include <button.h>
#include <wrap_menu.h>
#include <ufo_maths.h>
#include <ufo_engine.h>
#include <level.h>
#include <asset_manager.h>
#include "level_select_menu.h"
#include "new_game_menu.h"
#include "load_game_menu.h"

PingusMainMenu::PingusMainMenu(Vector2f _local_position) : WrapMenu(_local_position,Vector2f(250.0f,200.0f)){
    spacing = 8;
}

void PingusMainMenu::OnLevelEnter(Level* _level){
    WrapMenu::OnLevelEnter(_level);

    Engine::Get().active_profile = "default";

    if(AssetManager::Get().current_music_track != "Pingus_New_theme.wav"){
        AssetManager::Get().LoadAudio("../res/audio/Pingus_New_theme.wav", "Pingus_New_theme.wav");
        AssetManager::Get().PlayAudio("Pingus_New_theme.wav", true);
        AssetManager::Get().current_music_track = "Pingus_New_theme.wav";
    }

    has_modified_controls = true;
    
    dynamic_cast<PingusLevel*>(_level)->current_menu = this;

    level = dynamic_cast<PingusLevel*>(_level);
    dynamic_cast<PingusLevel*>(_level)->is_menu = true;

    auto b_new_game = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "New Game");

    b_new_game->on_pressed = [](Widget* _parent_widget, Button* _button){
        Console::PrintLine("New Game pressed");
        Engine::Get().current_level->NewActor<NewGameMenu>(_parent_widget->local_position);
        _parent_widget->QueueForPurge();
    };

    auto b_load_game = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Load Game");

    b_load_game->on_pressed = [](Widget* _parent_widget, Button* _button){
        Console::PrintLine("Load Game pressed");
        Engine::Get().current_level->NewActor<LoadGameMenu>(_parent_widget->local_position);
        _parent_widget->QueueForPurge();
    };

    auto b_level_select = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Level Select");
    auto b_options = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Options");

    auto b_level_editor = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Editor");
    b_level_editor->adjust_height_after_text_rows = true;

    b_level_editor->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), "../res/map/level_editor/level_editor.json");
    };

    auto b_quit = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Quit");

    b_level_select->on_pressed = [](Widget* _parent_widget, Button* _button){
        Console::PrintLine("Level select pressed");
        Engine::Get().current_level->NewActor<LevelSelectMenu>();
        _parent_widget->QueueForPurge();
    };

    b_quit->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().Quit();
    };

    b_new_game->adjust_height_after_text_rows = true;
    b_load_game->adjust_height_after_text_rows = true;
    b_level_select->adjust_height_after_text_rows = true;
    b_options->adjust_height_after_text_rows = true;
    b_quit->adjust_height_after_text_rows = true;

    AddChild(std::move(b_level_select));
    AddChild(std::move(b_quit));

    local_position.x = 40.0f;
    local_position.y = 300.0f;
    
}

void PingusMainMenu::OnStart(Level* _level){
    for(auto&& button : buttons){
        button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
        button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
    }
}

void PingusMainMenu::OnUpdate(){
    WrapMenu::OnUpdate();

    ControlWithMouse();
    ControlWithKeys(
        SingleKeyboard::Get().GetKey(olc::UP).is_pressed, SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed,
        SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed
    );

    //level->GetActiveCamera()->local_position = Vector2f(1100.0f, 200.0f);
    //level->GetActiveCamera()->scale = 0.5f;
    
}
