#include <memory>
#include <functional>
#include "../widgets/button.h"
#include "../widgets/wrap_menu.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../level/level.h"
#include "tuxtale_level.h"
#include "main_menu.h"
#include "../console/console.h"

std::unique_ptr<WrapMenu> PauseMenu(){
    auto pm = std::make_unique<WrapMenu>(Vector2f(40.0f, 40.0f),Vector2f(150.0f, 150.0f));

    Console::Out("id of pause menu:",pm->id);

    auto b_resume = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Resume");
    auto b_back_to_main_menu = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Back to main menu");

    b_resume->on_pressed = [&](Widget* _parent_widget, Button* _button){
        _parent_widget->QueueForPurge();
    };
    b_back_to_main_menu->on_pressed = [&](Widget* _parent_widget, Button* _button){
        Engine::Get().GoToLevel(MainMenu(),"../res/engine/default_level.json");
    };

    pm->AddChild(std::move(b_resume));
    pm->AddChild(std::move(b_back_to_main_menu));

    return std::move(pm);
}