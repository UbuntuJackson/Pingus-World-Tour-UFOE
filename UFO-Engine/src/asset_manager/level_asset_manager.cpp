#include "../external/olcPixelGameEngine.h"
#include <memory>
#include "level_asset_manager.h"
#include "../console/console.h"
#include <map>
#include "../ufo_engine/ufo_engine.h"
#include "../shapes/rectangle.h"
#include "../external/olcSoundWaveEngine.h"

std::map<std::string, std::unique_ptr<olc::Sprite>>& LevelAssetManager::GetSprites(){
    return sprites;
}

void LevelAssetManager::LoadSprite(const std::string& _path, const std::string& _name) {
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
LevelAssetManager::GetSprite(std::string _name) {
    if(!sprites.count(_name)) Console::Out("Sprite with key: " + _name + " does not exist");
    return sprites.at(_name).get();
}

std::map<std::string, std::unique_ptr<olc::Decal>>& LevelAssetManager::GetDecals(){
    return decals;
}

void LevelAssetManager::LoadDecal(const std::string& _path, const std::string& _name) {
    olc::Sprite* spr = new olc::Sprite(_path);

    switch (spr->LoadFromFile(_path)){
        case olc::rcode::NO_FILE:
            Console::Out("Error: The file does not exist at path", _path);
            return;
            break;
        case olc::rcode::FAIL:
            Console::Out("Error: Failed to load the file at path", _path);
            return;
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
LevelAssetManager::GetDecal(std::string _name) {
    if(!decals.count(_name)){
        Console::Out("Decal with key: " + _name + " does not exist");
        return nullptr;
        
    }
    return decals.at(_name).get();
}

void
LevelAssetManager::RemoveAsset(std::string _name){
    bool sprite_was_deleted = sprites.erase(_name);
    bool decal_was_deleted = decals.erase(_name);

    if(!sprite_was_deleted) Console::Out("Sprite with key: " + _name + " does not exist");
    if(!decal_was_deleted){
        if(!sprite_was_deleted) Console::Out("Decal with key: " + _name + " does not exist");
        else Console::Out("Only sprite was loaded for: " + _name);
    }
}

void LevelAssetManager::PlayAudio(std::string _name, bool _looping){
    audio_wave_form.emplace(_name, Engine::Get().wave_engine.PlayWaveform(audio.at(_name).get(), _looping));
}

void LevelAssetManager::LoadAudio(std::string _path, std::string _name){
    auto audio_wave_form = std::make_unique<olc::sound::Wave>();
    audio_wave_form->LoadAudioWaveform(_path);
    audio.emplace(_name, std::move(audio_wave_form));
}

olc::sound::Wave* LevelAssetManager::GetAudio(std::string _name){
    return audio.at(_name).get();
}

void LevelAssetManager::StopAudio(std::string _name){
    auto wf = audio_wave_form.at(_name);
    Engine::Get().wave_engine.StopWaveform(wf);
}

Rectangle
LevelAssetManager::GetFrameFromSpriteSheet(std::string _sprite_key, int _frame, olc::vf2d _frame_size){
    return GetRectangle(
        (int)_frame % (GetDecal(_sprite_key)->sprite->Size().x/(int)_frame_size.x), //1 can only give me x = 0
        (int)_frame / (GetDecal(_sprite_key)->sprite->Size().x/(int)_frame_size.x),
        _frame_size); //1 can only give y = 1
}