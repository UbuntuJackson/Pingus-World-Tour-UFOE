#include <string>
#include <button.h>
#include <ufo_maths.h>
#include <wrap_menu.h>
#include <console.h>
#include <nine_patch_theme.h>
#include <file.h>
#include <json.h>
#include <profile.h>
#include "pingus_level.h"
#include "load_game_menu.h"
#include "pingus_main_menu.h"

LoadGameMenu::LoadGameMenu(Vector2f _local_position) : WrapMenu(_local_position,Vector2f(250.0f,200.0f)){
    spacing = 8;
}

void LoadGameMenu::OnLevelEnter(Level* _level){
    WrapMenu::OnLevelEnter(_level);

    has_modified_controls = true;

    level = _level;

    auto b_save1 = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Save 1");
    auto b_save2 = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Save 2");
    auto b_save3 = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Save 3");
    auto b_back = AddChild<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 0.0f), "Back");

    b_back->on_pressed = [](Widget* _parent_widget, Button* _button){
        _parent_widget->QueueForPurge();
        Engine::Get().current_level->NewActor<PingusMainMenu>(_parent_widget->local_position);
    };

    b_save1->on_pressed = [](Widget* _parent_widget, Button* _button){
        dynamic_cast<LoadGameMenu*>(_parent_widget)->LoadSave("../res/save/save1.json", "save1");
    };

    b_save2->on_pressed = [](Widget* _parent_widget, Button* _button){
        dynamic_cast<LoadGameMenu*>(_parent_widget)->LoadSave("../res/save/save2.json", "save2");
    };

    b_save3->on_pressed = [](Widget* _parent_widget, Button* _button){
        dynamic_cast<LoadGameMenu*>(_parent_widget)->LoadSave("../res/save/save3.json", "save3");
    };

    for(auto&& button : buttons){
        button->adjust_height_after_text_rows = true;
        button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
        button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
    }

}

void LoadGameMenu::OnStart(Level* _level){
    
}

void LoadGameMenu::OnUpdate(){
    WrapMenu::OnUpdate();
    ControlWithMouse();
    ControlWithKeys(
        SingleKeyboard::Get().GetKey(olc::UP).is_pressed, SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed,
        SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed
    );
}

void LoadGameMenu::LoadSave(std::string _path, std::string _profile_name){
    std::string save_file_path = _path;
    JsonDictionary potentially_existing_save_json = JsonVariant::Read(save_file_path);
    
    if(!potentially_existing_save_json.IsNull()){
        
        Console::PrintLine("Reading from file", save_file_path, "...");

        Engine::Get().NewProfile(_profile_name,save_file_path);
        Engine::Get().active_profile = _profile_name;
        Engine::Get().GetActiveProfile()->save_file = JsonVariant::Read(save_file_path);
        Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), "../res/map/world_map/world_map.json");
    }
    else{
        Console::PrintLine("File",save_file_path,"does not exist.");
    }
}