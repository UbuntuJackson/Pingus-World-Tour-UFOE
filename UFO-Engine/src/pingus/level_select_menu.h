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
        
    }

    void OnFileMenuCreated(){
        
    }

    void OnFileOpen(FileMenuButton* _button,std::string _file_extension){
        Console::Out("OnFileOpen");
        if(_file_extension == "json"){
            Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), _button->path, Engine::LevelFormat::JSON_CUSTOM);
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
        //Console::Out("LevelSelectMenu::OnUpdate");
    }

    void OnPurge(){
        Console::Out("Purged");
    }

};