
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "widget_sprite_reference.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"
#include "../asset_manager/asset_manager.h"
#include "../ufo_engine/ufo_engine.h"

WidgetSpriteReference::WidgetSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, bool _temporary) :
    SpriteReference(_key, _position, _offset, _frame_size, _scale, _rotation, _z_index, _temporary){}

void
WidgetSpriteReference::OnDraw(Camera* _camera){

}

void
WidgetSpriteReference::OnWidgetDraw(){
    if(!visible) return;
    ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(key,current_frame_index,frame_size);
    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
        GetGlobalPosition(),
        AssetManager::Get().GetDecal(key),
        rotation,
        offset,
        sample_rectangle.position,
        sample_rectangle.size,
        scale,
        tint
    ); 
}

std::string WidgetSpriteReference::GetType(){
    return "WidgetSpriteReference";
}