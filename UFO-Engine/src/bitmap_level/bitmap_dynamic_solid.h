#ifndef DYNAMIC_SOLID_H
#define DYNAMIC_SOLID_H
#include "../bitmap_level/bitmap_collision_body.h"
#include <string>
#include "../external/olcPixelGameEngine.h"
class Camera;
class Level;

class BitmapDynamicSolid : public BitmapCollisionBody{
public:
    float angle = 0;
    bool was_updated = false;
    //std::string mask;
    int timelapse;
    BitmapDynamicSolid(olc::vf2d _position,Level* _level, std::string _mask, std::string _layer_tag);
    olc::vf2d GetVelocity();
    olc::vf2d GetPosition();
    void Update();
    void Draw(Camera* _camera);
};
#endif