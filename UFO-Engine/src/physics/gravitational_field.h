#include "../ufo_maths/ufo_maths.h"

class GravitationalFieldSingleDirectional{
public:
    olc::vf2d direction = ufoConstants::SCREEN_DOWN;
};

class GravitationalFieldPlanet{
public:
    olc::vf2d position;
    olc::vf2d GetGravitationalDirection(olc::vf2d _position){
        return position-_position;
    }
};