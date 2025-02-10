#ifndef LOCAL_POSITION
#define LOCAL_POSITION

#include "../external/olcPixelGameEngine.h"

class LocalPosition{
public:
    olc::vf2d* global_position;
    olc::vf2d offset;
    LocalPosition(olc::vf2d* _assumed_parent_position, olc::vf2d _offset);
    olc::vf2d GetGlobalPosition();
    olc::vf2d GetLocalPosition();
};

#endif