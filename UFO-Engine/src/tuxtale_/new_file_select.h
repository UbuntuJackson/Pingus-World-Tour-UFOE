#include "main_menu.h"
#include <memory>
#include <functional>
#include "../widgets/button.h"
#include "../widgets/wrap_menu.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../level/level.h"
#include "tuxtale_level.h"
#include "main_menu.h"
#include "../profile/profile.h"

std::unique_ptr<Level> NewFileSelect(){
    Engine::Get().NewProfile("File 1", "../res/tuxtale/res/save/save1.json");
    Engine::Get().NewProfile("File 2", "../res/tuxtale/res/save/save2.json");
    Engine::Get().NewProfile("File 3", "../res/tuxtale/res/save/save3.json");

    auto fm = std::make_unique<WrapMenu>(Vector2f(40.0f, 40.0f),Vector2f(150.0f, 150.0f));

    auto f1 = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "File 1");
    auto f2 = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "File 2");
    auto f3 = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "File 3");
    auto back = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Back");

    f1->on_pressed = [&](Widget* _parent_widget, Button* _button){
        Engine::Get().active_profile = "File 1";
        Engine::Get().GoToLevel("../res/tuxtale/res/tiles/green_maze.json");
    };

    f2->on_pressed = [&](Widget* _parent_widget, Button* _button){
        Engine::Get().active_profile = "File 2";
        Engine::Get().GoToLevel("../res/tuxtale/res/tiles/green_maze.json");
    };

    f3->on_pressed = [&](Widget* _parent_widget, Button* _button){
        Engine::Get().active_profile = "File 3";
        Engine::Get().GoToLevel("../res/tuxtale/res/tiles/green_maze.json");
    };

    back->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().GoToLevel(MainMenu(),"../res/engine/default_level.json");
    };

    fm->AddChild(std::move(f1));
    fm->AddChild(std::move(f2));
    fm->AddChild(std::move(f3));
    fm->AddChild(std::move(back));

    auto u_level = std::make_unique<TuxTaleLevel>();

    u_level->NewActor(std::move(fm));

    return std::move(u_level);
}