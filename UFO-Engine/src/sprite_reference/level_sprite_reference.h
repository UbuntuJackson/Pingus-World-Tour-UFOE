#ifndef LEVEL_SPRITE_REFERENCE
#define LEVEL_SPRITE_REFERENCE

#include <string>
#include "sprite_reference.h"
#include "../external/olcPixelGameEngine.h"

class Camera;
class LevelAssetManager;
class Level;

class LevelSpriteReference : public SpriteReference{
public:
    LevelAssetManager* level_asset_manager = nullptr;
    LevelSpriteReference(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, bool _temporary = false);
    void OnLevelEnter(Level* _level);
    void OnDraw(Camera*);
    std::string GetType();
};

#endif