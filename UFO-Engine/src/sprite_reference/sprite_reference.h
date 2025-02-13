#ifndef SPRITE_REFERENCE_H
#define SPRITE_REFERENCE_H
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "../actor/actor.h"

class Camera;

class SpriteReference : public Actor{
public:
    std::string key;
    olc::vf2d offset;
    olc::vf2d frame_size;
    olc::vf2d scale = {1.0f, 1.0f};
    float rotation = 0.0f;
    int z_index = 0;
    float current_frame_index = 0;
    int number_of_frames;
    bool temporary;
    bool visible = true;
    olc::Pixel tint = olc::WHITE;

    SpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index = 0, bool _temporary = false, bool _custom_asset_manager = false);
    
    void OnLevelEnter(Level* _level);

    void OnDraw(Camera* _camera);
    
};

#endif