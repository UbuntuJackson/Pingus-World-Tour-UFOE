#pragma once
#include "../external/olcPixelGameEngine.h"
#include "../external/olcSoundWaveEngine.h"
#include <string>
#include <map>
#include <memory>

class AbstractAssetManager{
public:
    AbstractAssetManager() = default;

    std::string current_music_track = "";

    /*std::map<std::string, std::unique_ptr<olc::Sprite>> sprites;
    std::map<std::string,std::unique_ptr<olc::Decal>> decals;
    std::map<std::string,std::unique_ptr<olc::sound::Wave>> audio;
    std::map<std::string,olc::sound::PlayingWave> audio_wave_form;*/

    std::map<std::string, std::unique_ptr<olc::Sprite>>& GetSprites();
    virtual void LoadSprite(const std::string& _path, const std::string& _name);
    virtual olc::Sprite* GetSprite(std::string _name);
    virtual std::map<std::string, std::unique_ptr<olc::Decal>>& GetDecals();
    virtual void LoadDecal(const std::string& _path, const std::string& _name); //wouldn't this load both a sprite and a decal? And should the load functions return something?
    virtual olc::Decal* GetDecal(std::string _name);
    virtual void RemoveAsset(std::string _name);

    virtual void PlayAudio(std::string _name, bool _looping);
    virtual void LoadAudio(std::string _path, std::string _name);
    virtual olc::sound::Wave* GetAudio(std::string _name);
    virtual void StopAudio(std::string _name);

};