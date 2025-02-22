#ifndef COLLISION_RECTANGLE
#define COLLISION_RECTANGLE

#include "../external/olcPixelGameEngine.h"
#include "rectangle.h"

class AlternativeCollisionBody;
class Camera;

class CollisionRectangle : public ufo::Rectangle{
public:
    CollisionRectangle(AlternativeCollisionBody* _collision_body, olc::vf2d _position, olc::vf2d _size);
    void Update();
    virtual void Draw(Camera* _camera);
};

#endif