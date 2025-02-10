#ifndef WIDGET_SPRITE_REFERENCE
#define WIDGET_SPRITE_REFERENCE

#include <string>
#include "sprite_reference.h"
#include "../external/olcPixelGameEngine.h"

class Camera;

class WidgetSpriteReference : public SpriteReference{
public:
    WidgetSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, bool _temporary = false);
    void OnDraw(Camera*);
    void OnWidgetDraw();
    std::string GetType();
};

#endif