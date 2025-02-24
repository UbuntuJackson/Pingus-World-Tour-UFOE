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
#include <level_sprite_reference.h>

class LevelSelectMenu : public FileMenu{
public:
    LevelSelectMenu() : FileMenu(Vector2f(40.0f, 40.0f),Vector2f(250.0f, 400.0f), "../res"){
        has_modified_controls = true;
        spacing = 8;
    }

    void OnFileMenuCreated(){
        
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
            Console::Out("preview.png");
            AddChild<LevelSpriteReference>(
                _path+"/"+"preview.png",
                Vector2f(800.0f,157.0f),
                Vector2f(0.0f,0.0f),
                Vector2f(200.0f,200.0f),
                Vector2f(1.0f,1.0f),
                0.0f
            )->SetZIndex(2);
        }
        if(path_stack.size() == 0){
            auto bback = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 150.0f), "Back", path);

            bback->background_colour = olc::Pixel(200,90,0);

            bback->on_pressed = [](Widget* _w, Button* _button){
                _w->QueueForPurge();
                _w->level->NewActor<PingusMainMenu>(Vector2f(0.0f,0.0f));
            };

            buttons.push_back(bback.get());

            AddChild(std::move(bback));

            float total_height = 0.0f;

            for(const auto& button : buttons){
                button->local_position.y = total_height;
                total_height+=button->rectangle.size.y;
                total_height+=spacing;
            }

            rectangle.size.y = total_height;
            rectangle.size.x = 0.0f;

            original_position = local_position;

            for(auto button : buttons){
                button->theme = OnSetButtonTheme();
                button->hovered_theme = OnSetButtonHoveredTheme();
                button->held_theme = OnSetButtonHeldTheme();
            }
        }
    }

    void OnUpdate(){
        FileMenu::OnUpdate();

        ControlWithMouse();
        ControlWithKeys(
            SingleKeyboard::Get().GetKey(olc::UP).is_pressed, SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed,
            SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed
        );

        level->GetActiveCamera()->local_position = Vector2f(800.0f, 400.0f);
        
        //Console::Out("LevelSelectMenu::OnUpdate");
    }

    void OnPurge(){
        Console::Out("Purged");
    }

};