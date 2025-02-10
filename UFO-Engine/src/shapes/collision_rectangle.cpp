#include "../drawing_system/drawing_system.h"
#include "rectangle.h"
#include "collision_rectangle.h"
#include "../camera/camera.h"
#include "../collision/collision_body.h"

CollisionRectangle::CollisionRectangle(AlternativeCollisionBody* _collision_body, olc::vf2d _position, olc::vf2d _size) : Rectangle(_position, _size){}

void CollisionRectangle::Update(){

}

void CollisionRectangle::Draw(Camera *_camera){
    DrawingSystem::Draw(*this,_camera, colour);
}