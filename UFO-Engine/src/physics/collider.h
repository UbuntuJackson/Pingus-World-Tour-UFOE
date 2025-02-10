#ifndef COLLIDER_H
#define COLLIDER_H
#include "../external/olcPixelGameEngine.h"

class ColliderBase{
public:
    virtual void Update(CollisionSystem* _cs) = 0;
}

template<typename tShape>
class Collider : ColliderBase{
public:
    tShape* shape;
    olc::vf2d position; //only useful if you want the camera to follow it
    olc::vf2d offset;  //only useful if you want the camera to follow it
    olc::vf2d velocity;
    Collider(tShape* _shape, olc::vf2d _velocity){
        shape = _shape;
        velocity = _velocity;
    }
    void Update(CollisionSystem* _cs){
        _cs->HandleSingleObjectCollision(shape, &velocity);
    }
};

namespace ufoStandard{
    CreationData<Collider<Rectangle>> NewColliderRectangle(Scene* _scene, olc::vf2d _velocity, float _x, float _y, float _width, float _height);
    CreationData<Collider<Circle>> NewColliderCircle(Scene* _scene, olc::vf2d _velocity, float _x, float _y, float _radius);
}

#endif