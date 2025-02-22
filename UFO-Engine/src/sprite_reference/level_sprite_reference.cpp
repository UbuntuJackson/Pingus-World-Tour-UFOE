#include <string>
#include "../external/olcPixelGameEngine.h"
#include "level_sprite_reference.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"
#include "../asset_manager/asset_manager.h"
#include "../ufo_engine/ufo_engine.h"
#include "../shapes/rectangle.h"
#include "../asset_manager/get_rect_from_index.h"
#include "../level/level.h"

LevelSpriteReference::LevelSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, bool _temporary) :
    SpriteReference(_key, _position, _offset, _frame_size, _scale, _rotation, _temporary, 0,true){}

void LevelSpriteReference::OnLevelEnter(Level* _level){
    SpriteReference::OnLevelEnter(_level);
    level_asset_manager = &_level->asset_manager;
    number_of_frames = level_asset_manager->GetDecal(key)->sprite->Size().x/(int)frame_size.x * level_asset_manager->GetDecal(key)->sprite->Size().y/(int)frame_size.y;
}

void
LevelSpriteReference::OnDraw(Camera* _camera){
    if(!visible) return;
    ufo::Rectangle sample_rectangle = level_asset_manager->GetFrameFromSpriteSheet(key,current_frame_index,frame_size);
    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
        _camera->Transform(GetGlobalPosition()),
        level_asset_manager->GetDecal(key),
        rotation,
        offset,
        sample_rectangle.position,
        sample_rectangle.size,
        scale*_camera->scale,
        tint
    );
}

std::string LevelSpriteReference::GetType(){
    return "LevelSpriteReference";
}