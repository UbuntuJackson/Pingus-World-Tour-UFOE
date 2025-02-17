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
#include <file_menu.h>

class LevelSelectMenu : public FileMenu{
public:
    LevelSelectMenu() : FileMenu(Vector2f(40.0f, 0.0f),Vector2f(250.0f, 200.0f), "../res"){
        has_modified_controls = true;
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

    void OnUpdate(){
        FileMenu::OnUpdate();

        ControlWithMouse();
        ControlWithKeys(
            SingleKeyboard::Get().GetKey(olc::UP).is_pressed, SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed,
            SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed
        );

        level->GetActiveCamera()->local_position = Vector2f(800.0f, 400.0f);
        level->GetActiveCamera()->scale = 0.5f;
        //Console::Out("LevelSelectMenu::OnUpdate");
    }

    void OnPurge(){
        Console::Out("Purged");
    }

};