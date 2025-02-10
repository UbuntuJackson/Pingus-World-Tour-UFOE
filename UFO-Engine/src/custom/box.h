#ifndef BOX_H
#define BOX_H

#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "../json/json.h"

class Level;
class Frogatto;

class Box : public Shape<Rectangle>{
public:
    Box(olc::vf2d _position);
    void OnLevelEnter(Level* _level);
    void OnEaten(Frogatto* _frogatto);
    static std::unique_ptr<Box> Load(Json& _json);
};

#endif