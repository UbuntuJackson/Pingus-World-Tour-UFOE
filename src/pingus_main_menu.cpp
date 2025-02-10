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

PingusMainMenu::PingusMainMenu(Vector2f _local_position) : WrapMenu(_local_position,Vector2f(250.0f,200.0f)){

}

void PingusMainMenu::OnLevelEnter(Level* _level){
    dynamic_cast<PingusLevel*>(_level)->current_menu = this;

    level = dynamic_cast<PingusLevel*>(_level);

    auto b_level_select = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Level Select");
    auto b_quit = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Quit");

    b_level_select->on_pressed = [](Widget* _parent_widget, Button* _button){
        Console::Out("Level select pressed");
        _parent_widget->level->NewActor<LevelSelectMenu>();
        _parent_widget->QueueForPurge();
    };

    b_quit->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().Quit();
    };

    AddChild(std::move(b_level_select));
    AddChild(std::move(b_quit));

    WrapMenu::OnLevelEnter(_level);
    
}
