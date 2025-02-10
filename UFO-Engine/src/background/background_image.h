#ifndef BACKGROUND_IMAGE
#define BACKGROUND_IMAGE
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "background_image.h"
#include "../sprite_reference/sprite_reference.h"

class Camera;

class BackgroundImage : public SpriteReference{
public:
    olc::vf2d parallax;
    BackgroundImage(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, olc::vf2d _parallax, bool _temporary = false);

    void OnDraw(Camera* _camera);
    void OnWidgetDraw();
    void OnBackgroundDraw(Camera* _camera);
};

#endif