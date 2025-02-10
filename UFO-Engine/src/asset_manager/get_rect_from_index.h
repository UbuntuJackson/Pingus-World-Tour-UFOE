#ifndef GET_RECT_FROM_INDEX_H
#define GET_RECT_FROM_INDEX_H

#include "asset_manager.h"
#include "../shapes/rectangle.h"

Rectangle
GetRectangle(int _x, int _y, olc::vf2d _frame_size);

Rectangle
GetFrameFromSpriteSheet(std::string _sprite_key, int _frame, olc::vf2d _frame_size);

#endif