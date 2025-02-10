
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "widget_sprite_reference.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"

WidgetSpriteReference::WidgetSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, bool _temporary) :
    SpriteReference(_key, _position, _offset, _frame_size, _scale, _rotation, _z_index, _temporary){}

void
WidgetSpriteReference::OnDraw(Camera* _camera){

}

void
WidgetSpriteReference::OnWidgetDraw(){
    if(visible) DrawingSystem::Draw(*this);
}

std::string WidgetSpriteReference::GetType(){
    return "WidgetSpriteReference";
}