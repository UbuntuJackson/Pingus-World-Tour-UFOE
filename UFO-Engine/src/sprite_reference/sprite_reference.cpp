#include <string>
#include "../external/olcPixelGameEngine.h"
#include "sprite_reference.h"
#include "../actor/actor.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"

SpriteReference::SpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, bool _temporary, bool _custom_asset_manager) :
    key{_key},
    Actor(_position),
    offset{_offset},
    frame_size{_frame_size},
    scale{_scale},
    rotation{_rotation},
    z_index{_z_index},
    temporary{_temporary}
    {
        //if(!_temporary) Engine::Get().current_level->sprite_handles.push_back(this);
        if(!_custom_asset_manager) number_of_frames = AssetManager::Get().GetDecal(_key)->sprite->Size().x/(int)_frame_size.x * AssetManager::Get().GetDecal(_key)->sprite->Size().y/(int)_frame_size.y;
    }

void SpriteReference::OnLevelEnter(Level* _level){
    if(!temporary) _level->sprite_handles.push_back(this);
}

void SpriteReference::OnDraw(Camera* _camera){

    if(!visible) return;
    ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(key,current_frame_index,frame_size);
    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
        _camera->Transform(GetGlobalPosition()),
        AssetManager::Get().GetDecal(key),
        rotation,
        offset,
        sample_rectangle.position,
        sample_rectangle.size,
        scale*_camera->scale,
        tint
    );

}