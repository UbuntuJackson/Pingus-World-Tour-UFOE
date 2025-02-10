#ifndef COIN_H
#define COIN_H
#include "../external/olcPixelGameEngine.h"

struct Coin{
    int value;
    olc::vf2d position;
    Coin(olc::vf2d _position, int _value) : position{_position}, value{_value}{}
};

#endif