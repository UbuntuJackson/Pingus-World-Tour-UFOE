#include "main_menu.h"
#include <memory>
#include <functional>
#include "../widgets/button.h"
#include "../widgets/wrap_menu.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../level/level.h"
#include "tuxtale_level.h"
#include "load_file_select.h"
#include "new_file_select.h"
#include "../asset_manager/asset_manager.h"

std::unique_ptr<Level> MainMenu(){
    
    if(AssetManager::Get().current_music_track != "quest_for_everything"){
        Engine::Get().wave_engine.StopAll();

        AssetManager::Get().LoadAudio("../res/tuxtale/res/audio/Quest_for_Everything.wav","quest_for_everything");

        AssetManager::Get().PlayAudio("quest_for_everything", true);
        
        AssetManager::Get().current_music_track = "quest_for_everything";
    }

    auto mm = std::make_unique<WrapMenu>(Vector2f(100.0f, 100.0f),Vector2f(150.0f, 70.0f));

    auto b_new = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "New Game");
    auto b_load = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Load Game");
    auto b_options = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Options");
    auto b_quit = std::make_unique<Button>(Vector2f(0.0f, 0.0f),Vector2f(150.0f, 150.0f), "Quit");

    b_new->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().GoToLevel(NewFileSelect(), "../res/engine/default_level.json");
    };
    b_load->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().GoToLevel(LoadFileSelect(), "../res/engine/default_level.json");
    };
    b_quit->on_pressed = [](Widget* _parent_widget, Button* _button){
        Engine::Get().Quit();
    };

    mm->AddChild(std::move(b_new));
    mm->AddChild(std::move(b_load));
    mm->AddChild(std::move(b_options));
    mm->AddChild(std::move(b_quit));

    auto u_level = std::make_unique<TuxTaleLevel>();

    u_level->NewActor(std::move(mm));

    return std::move(u_level);
}