#include "get_rect_from_index.h"
#include "asset_manager.h"
#include "../shapes/rectangle.h"
#include <string>

ufo::Rectangle
GetRectangle(int _x, int _y, olc::vf2d _frame_size){
    ufo::Rectangle rect = ufo::Rectangle({(float)(_x * _frame_size.x), (float)(_y * _frame_size.y)}, _frame_size);
    return rect;
}

ufo::Rectangle
GetFrameFromSpriteSheet(std::string _sprite_key, int _frame, olc::vf2d _frame_size){
    return GetRectangle(
        (int)_frame % (AssetManager::Get().GetDecal(_sprite_key)->sprite->Size().x/(int)_frame_size.x), //1 can only give me x = 0
        (int)_frame / (AssetManager::Get().GetDecal(_sprite_key)->sprite->Size().x/(int)_frame_size.x),
        _frame_size); //1 can only give y = 1
}