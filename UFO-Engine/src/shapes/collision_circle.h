#ifndef COLLISION_CIRCLE
#define COLLISION_CIRCLE

#include "../external/olcPixelGameEngine.h"
#include "circle.h"

class CollisionBody;
class Camera;

class CollisionCircle : public Circle{
public:
    bool collision_happened_this_frame = false;
    bool collision_happened_last_frame = false;
    CollisionBody* collision_body;
    CollisionCircle(CollisionBody* _collision_body, olc::vf2d _position, float _radius);
    void Update();
    virtual void Draw(Camera* _camera);
};

#endif