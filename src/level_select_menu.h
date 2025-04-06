#pragma once

#include <memory>
#include <filesystem>
#include <functional>
#include <button.h>
#include <wrap_menu.h>
#include <ufo_maths.h>
#include <ufo_engine.h>
#include <level.h>
#include "pingus_level.h"
#include "pingus_main_menu.h"
#include <file_menu.h>
#include <nine_patch_theme.h>
#include <level_widget_sprite_reference.h>

class LevelSelectMenu : public FileMenu{
public:

    Level* level = nullptr;

    LevelSelectMenu() : FileMenu(Vector2f(40.0f, 40.0f),Vector2f(250.0f, 400.0f), "../res/map"){
        has_modified_controls = true;
        spacing = 8;
    }

    void OnLevelEnter(Level* _level){
        FileMenu::OnLevelEnter(_level);
        level = _level;
    }

    void OnFileOpen(FileMenuButton* _button,std::string _file_extension){
        Console::Out("OnFileOpen");
        if(_file_extension == "json"){
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), _button->path);
        }
        else{
            Console::Out("File is not .json");
        }

        if(_file_extension == "cpp"){
            Console::Out(File().Read(_button->path));

            level->NewActor(std::make_unique<Label>(Vector2f(400.0f, 0.0f), Vector2f(400.0f, 400.0f), File().Read(_button->path)));
        }
    }

    std::unique_ptr<Theme> OnSetButtonTheme(){
        return std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    }

    std::unique_ptr<Theme> OnSetButtonHoveredTheme(){
        return std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    }

    std::unique_ptr<Theme> OnSetButtonHeldTheme(){
        return std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
    }

    void OnSetDirectory(std::string _path){
        Console::Out(_path);
        level->asset_manager.LoadDecal(_path+"/"+"preview.png",_path+"/"+"preview.png");
        if(level->asset_manager.GetDecal(_path+"/"+"preview.png")){
            
            AddChild<LevelWidgetSpriteReference>(
                _path+"/"+"preview.png",
                Vector2f(340.0f,0.0f),
                Vector2f(0.0f,0.0f),
                Vector2f(200.0f,200.0f),
                Vector2f(1.0f,1.0f),
                0.0f
            )->SetZIndex(2);
        }

        //Need a buton to go back when file menu does not have a previous path.
        if(path_stack.size() == 0){
            Console::Print("Back button spawned\n");
            auto bback = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 20.0f), "Back", path);

            bback->on_pressed = [](Widget* _w, Button* _button){
                _w->QueueForPurge();
                Engine::Get().current_level->NewActor<PingusMainMenu>(Vector2f(0.0f,0.0f));
            };

            bback->adjust_height_after_text_rows = true;

            AddChild(std::move(bback));

            for(const auto& button : buttons){
                button->adjust_height_after_text_rows = true;
            }

        }

        Refresh();

        //Themes need to be added manually.
        for(auto button : buttons){
            button->theme = OnSetButtonTheme();
            button->hovered_theme = OnSetButtonHoveredTheme();
            button->held_theme = OnSetButtonHeldTheme();
        }
    }

    void OnUpdate(){
        FileMenu::OnUpdate();

        Console::PrintLine("Global Position of selected button:", selected_index, buttons[selected_index]->GetGlobalPosition());
        if(SingleKeyboard::Get().GetKey(olc::ESCAPE).is_pressed) Engine::Get().Quit();

        ControlWithMouse();
        ControlWithKeys(
            SingleKeyboard::Get().GetKey(olc::UP).is_pressed, SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed,
            SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed
        );

    }

};