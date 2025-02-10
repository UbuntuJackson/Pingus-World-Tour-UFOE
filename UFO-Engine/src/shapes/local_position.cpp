#include "local_position.h"
#include "../external/olcPixelGameEngine.h"

LocalPosition::LocalPosition(olc::vf2d* _global_position, olc::vf2d _offset) :
global_position{_global_position},
offset{_offset}{}

olc::vf2d LocalPosition::GetGlobalPosition(){
    return *global_position+offset;
}

olc::vf2d LocalPosition::GetLocalPosition(){
    return offset;
}