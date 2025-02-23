#include <memory>
#include "ufo_engine.h"
#include "../keyboard/single_keyboard.h"
#include "../mouse/mouse.h"
#include "../json/json.h"
#include "../custom/frogatto_level.h"
#include "../keyboard/single_keyboard.h"
#include "../json/json_variant.h"
//#include "../ufo_lang/ufo_lang.h"

Profile* Engine::GetActiveProfile(){
    return profiles.at(active_profile).get();
}

Profile* Engine::GetProfile(std::string _name){
    return profiles.at(_name).get();
}

void Engine::NewProfile(std::string _name, std::string _profile_path){
    profiles.emplace(_name, std::make_unique<Profile>(_profile_path));
}

void Engine::Quit(){quit = true;}
float Engine::GetDeltaTime(){return delta_time;}

void Engine::GoToLevel(std::unique_ptr<Level> _level, std::string _path, int _level_format){
    _level->path = _path;
    _level->level_format = _level_format;
    queued_levels.push_back(std::move(_level));
}

void Engine::GoToLevel(std::string _path, int _level_format){
    Json level_json = Json::Read(_path);
    std::string level_class_name = "";
    Json properties = level_json.GetObject("properties");
    Json::ArrayForEach(Json(),properties,[&](Json& _object, Json _objects){
        if(_object.GetAsString("name") == "level_class"){
            level_class_name = _object.GetAsString("value");
        }
    });

    auto l_level = (level_classes.at(level_class_name))();
    l_level->path = _path;
    l_level->level_format = _level_format;
    queued_levels.push_back(std::move(l_level));
}

bool
Engine::Start(){
    wave_engine.InitialiseAudio();

    JsonDictionary engine_config = JsonVariant::Read("../UFO-Engine/res/engine_config/config.json");

    JsonDictionary& resolution = engine_config.Get("resolution").AsDictionary();
    int res_width = resolution.Get("x").AsInt();
    int res_height = resolution.Get("y").AsInt();
    JsonDictionary& pixel_size = engine_config.Get("pixel-size").AsDictionary();
    int pixel_width = pixel_size.Get("x").AsInt();
    int pixel_height = pixel_size.Get("y").AsInt();
    vsync = (bool)(engine_config.Get("vsync").AsInt());
    edit_mode = (bool)(engine_config.Get("edit-mode").AsInt());
    all_shapes_visible = (bool)(engine_config.Get("all-shapes-visible").AsInt());
    modified_collision_lines_visible = (bool)(engine_config.Get("modified-collision-lines-visible").AsInt());
    if(edit_mode){
        pixel_width = 1;
        pixel_height = 1;
        res_width = 1600;
        res_height = 800;
    }

    NewProfile("Default", "../UFO-Engine/res/engine/default_profile.json");
    
    if(pixel_game_engine.Construct(res_width,res_height,pixel_width,pixel_height,false,vsync,true)){
        pixel_game_engine.engine = this;
        pixel_game_engine.Start();
        return false;
    }
    return true;
}

bool
Engine::OnReady(){
    return true;
}

bool
Engine::OnUpdate(float _delta_time){

    //if(SingleKeyboard::Get().GetKey(olc::ESCAPE).is_held) Quit();

    delta_time = _delta_time;
    pixel_game_engine.Clear(background_colour);
    pixel_game_engine.SetPixelMode(olc::Pixel::NORMAL);
    if(queued_levels.size() > 0){
        std::unique_ptr<Level> former_level = std::move(current_level);
        for(auto [k,v] : former_level->level_decals){
            AssetManager::Get().RemoveAsset(k);
        }
        current_level = std::move(queued_levels.back());
        current_level->Load();
        if(former_level->path != "") current_level->OnTransition(former_level.get());
        
        for(const auto& actor : current_level->transitionable_actor_handles){
            actor->OnTransition(former_level.get());
        }

        former_level->OnExit();
        queued_levels.pop_back();
    }
    current_level->Update();
    current_level->Draw();
    if(all_shapes_visible) current_level->DebugDraw();
    Mouse::Get().Update();
    //if(SingleKeyboard::Get().GetKey(olc::F1).is_pressed) pixel_game_engine.ConsoleShow(olc::F1);
    return !quit;
}

bool
Engine::OnUserCommand(std::string& _text){
    //ufoLang::Execute(scene_system.GetActiveScene(), _text);

    return true;
}