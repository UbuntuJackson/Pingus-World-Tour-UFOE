#include "profile.h"
#include <string>
#include "../json/json.h"
#include "../ufo_engine/ufo_engine.h"

Profile::Profile(std::string _file_path_and_name) : file_path_and_name{_file_path_and_name}{
    save_file = Json::Read(file_path_and_name);
}

void Profile::Save(){

    Engine::Get().current_level->Save(&save_file);
    Engine::Get().current_level->OnSave(&save_file);

    save_file.Write(file_path_and_name);
}

void Profile::Load(){

    save_file = Json::Read(file_path_and_name);

    Engine::Get().GoToLevel(save_file.GetAsString("level_path"));
    Engine::Get().queued_levels.back()->spawn_mode = SpawnMode::SAVEFILE;
    
}