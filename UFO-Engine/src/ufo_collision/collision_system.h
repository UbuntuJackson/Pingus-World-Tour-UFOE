#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../external/olcPixelGameEngine.h"

class Tilemap;
class Rectangle;

class CollisionSystem{
public:
    Tilemap* tilemap;
    CollisionSystem(Tilemap* _tilemap){
        tilemap = _tilemap;
    }
    void HandleSingleObjectCollision(Rectangle* _rectangle, olc::vf2d* _velocity){
        //Collision with tilemap
        //tilemap->
    }
};

#endif