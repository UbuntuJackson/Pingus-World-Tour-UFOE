#include "../external/olcPixelGameEngine.h"
#include <memory>
#include "asset_manager.h"
#include "../console/console.h"
#include <map>
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcSoundWaveEngine.h"

std::map<std::string, std::unique_ptr<olc::Sprite>>& AssetManager::GetSprites(){
    return sprites;
}

void AssetManager::LoadSprite(const std::string& _path, const std::string& _name) {
    olc::Sprite* spr = new olc::Sprite(_path);

    switch (spr->LoadFromFile(_path)){
        case olc::rcode::NO_FILE:
            Console::Out("Error: The file does not exist at path", _path);
            break;
        case olc::rcode::FAIL:
            Console::Out("Error: Failed to load the file at path", _path);
            break;
        default:
            break;
    }

    if(!sprites.count(_name)) sprites[_name] = std::unique_ptr<olc::Sprite>(spr);
    else Console::Out("Sprite with key: " + _name + " already exists");
}

olc::Sprite*
AssetManager::GetSprite(std::string _name) {
    if(!sprites.count(_name)) Console::Out("Sprite with key: " + _name + " does not exist");
    return sprites.at(_name).get();
}

std::map<std::string, std::unique_ptr<olc::Decal>>& AssetManager::GetDecals(){
    return decals;
}

void AssetManager::LoadDecal(const std::string& _path, const std::string& _name) {
    olc::Sprite* spr = new olc::Sprite(_path);

    switch (spr->LoadFromFile(_path)){
        case olc::rcode::NO_FILE:
            Console::Out("Error: The file does not exist at path", _path);
            break;
        case olc::rcode::FAIL:
            Console::Out("Error: Failed to load the file at path", _path);
            break;
        default:
            break;
    }

    olc::Decal* dec = new olc::Decal(spr);

    if(!sprites.count(_name)) sprites[_name] = std::unique_ptr<olc::Sprite>(spr);
    else Console::Out("Sprite with key: " + _name + " already exists");

    if(!decals.count(_name)) decals[_name] = std::unique_ptr<olc::Decal>(dec);
    else Console::Out("Decal with key: " + _name + " already exists");
}

olc::Decal*
AssetManager::GetDecal(std::string _name) {
    if(!decals.count(_name)) Console::Out("Decal with key: " + _name + " does not exist");
    return decals.at(_name).get();
}

void
AssetManager::RemoveAsset(std::string _name){
    bool sprite_was_deleted = sprites.erase(_name);
    bool decal_was_deleted = decals.erase(_name);

    if(!sprite_was_deleted) Console::Out("Sprite with key: " + _name + " does not exist");
    if(!decal_was_deleted){
        if(!sprite_was_deleted) Console::Out("Decal with key: " + _name + " does not exist");
        else Console::Out("Only sprite was loaded for: " + _name);
    }
}

void AssetManager::PlayAudio(std::string _name, bool _looping){
    audio_wave_form.emplace(_name, Engine::Get().wave_engine.PlayWaveform(audio.at(_name).get(), _looping));
}

void AssetManager::LoadAudio(std::string _path, std::string _name){
    auto audio_wave_form = std::make_unique<olc::sound::Wave>();
    audio_wave_form->LoadAudioWaveform(_path);
    audio.emplace(_name, std::move(audio_wave_form));
}

olc::sound::Wave* AssetManager::GetAudio(std::string _name){
    return audio.at(_name).get();
}

void AssetManager::StopAudio(std::string _name){
    auto wf = audio_wave_form.at(_name);
    Engine::Get().wave_engine.StopWaveform(wf);
}