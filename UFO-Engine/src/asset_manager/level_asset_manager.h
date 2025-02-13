#pragma once
#include "../external/olcPixelGameEngine.h"
#include "../external/olcSoundWaveEngine.h"
#include "../shapes/rectangle.h"
#include <string>
#include <map>
#include <memory>

class LevelAssetManager{
public:
    LevelAssetManager() = default;

    std::string current_music_track = "";

    std::map<std::string, std::unique_ptr<olc::Sprite>> sprites;
    std::map<std::string,std::unique_ptr<olc::Decal>> decals;
    std::map<std::string,std::unique_ptr<olc::sound::Wave>> audio;
    std::map<std::string,olc::sound::PlayingWave> audio_wave_form;

    std::map<std::string, std::unique_ptr<olc::Sprite>>& GetSprites();
    void LoadSprite(const std::string& _path, const std::string& _name);
    olc::Sprite* GetSprite(std::string _name);
    std::map<std::string, std::unique_ptr<olc::Decal>>& GetDecals();
    void LoadDecal(const std::string& _path, const std::string& _name); //wouldn't this load both a sprite and a decal? And should the load functions return something?
    olc::Decal* GetDecal(std::string _name);
    void RemoveAsset(std::string _name);

    void PlayAudio(std::string _name, bool _looping);
    void LoadAudio(std::string _path, std::string _name);
    olc::sound::Wave* GetAudio(std::string _name);
    void StopAudio(std::string _name);

    Rectangle GetFrameFromSpriteSheet(std::string _sprite_key, int _frame, olc::vf2d _frame_size);

};