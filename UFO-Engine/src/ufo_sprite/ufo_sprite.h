#ifndef SPRITE_H
#define SPRITE_H
#include <string>
#include "../external/olcPixelGameEngine.h"

class ufoSprite{
public:
    std::string key;
    olc::vf2d position;
    olc::vf2d *parent_position; //if pointer to position, then it just doesn't have a parent position.
    olc::vf2d offset;
    float scale;
    float rotation;
    olc::vf2d frame_size;
    int z;
    ufoSprite(
        std::string _key,
        olc::vf2d _position,
        olc::vf2d _frame_size,
        olc::vf2d _offset = {0.0f, 0.0f},
        float _scale = 1.0f,
        float _rotation = 0.0f,
        int _z = 0
    );

    void SetParentPosition(olc::vf2d* _parent_position);
};
#endif