#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>
#include <map>
#include <functional>
#include "../level/level.h"
#include "../graphics_provider/pixel_game_engine.h"
#include "../profile/profile.h"
#include "../external/olcSoundWaveEngine.h"

class Engine{
public:
    enum LevelFormat{
        JSON,
        JSON_CUSTOM
    };

    PixelGameEngine pixel_game_engine;
    olc::sound::WaveEngine wave_engine;
    olc::sound::Wave test_sound;
    olc::sound::PlayingWave wave_form;

    const int IMMORTAL_ID = 0;
    const int NONE_ID = -1;
    int id_counter = 1;
    int GenerateID(){
        return id_counter++;}
    bool quit = false;

    //Basically savefiles
    std::map<std::string, std::unique_ptr<Profile>> profiles;

    std::string active_profile = "Default";

    Profile* GetActiveProfile();

    Profile* GetProfile(std::string _name);

    void NewProfile(std::string _name, std::string _profile_path);

    std::map<std::string,std::function<std::unique_ptr<Level>()>> level_classes;

    

    template<typename tLevelType>
    void NewLevelClass(std::string _level_class_name){
        level_classes.emplace(_level_class_name,[](){return std::make_unique<tLevelType>();});
    }

    std::unique_ptr<Level> current_level = std::make_unique<Level>();
    std::vector<std::unique_ptr<Level>> queued_levels;
    void GoToLevel(std::unique_ptr<Level> _level, std::string _path, int _level_format = LevelFormat::JSON);
    void GoToLevel(std::string _path, int _level_format = LevelFormat::JSON);

    Engine(){
        NewLevelClass<Level>("Level");
    }

    static Engine& Get(){
        static Engine engine;
        return engine;
    }

    void Quit();

    float delta_time;
    float GetDeltaTime();

    bool all_shapes_visible = false;
    bool edit_mode = false;
    bool modified_collision_lines_visible = false;
    bool vsync = false;

    olc::Pixel background_colour = olc::Pixel(52,52,60);

    virtual bool Start();
    virtual bool OnReady();
    virtual bool OnUpdate(float _delta_time);
    virtual bool OnUserCommand(std::string& _text);
};

#endif