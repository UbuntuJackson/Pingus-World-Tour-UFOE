#include "bitmap_dynamic_solid.h"
#include "bitmap_collision_body.h"
#include <string>
#include "ufo_global.h"
#include "../camera/camera.h"
#include "../level/level.h"
#include <cmath>

BitmapDynamicSolid::BitmapDynamicSolid(olc::vf2d _position, Level* _level, std::string _mask, std::string _layer_tag) : BitmapCollisionBody(_position, _mask, _layer_tag){
    velocity = {0.0f, 5.3f};
    timelapse = 0;
    std::cout << "spawned dynamic solid" << std::endl;
}

olc::vf2d BitmapDynamicSolid::GetVelocity(){
    if(was_updated) angle-=0.1;
    velocity.x = 2.0f; //change vel after movement to avoid wacky inaccurate adding of velocity
    velocity.y = 5.0f * std::sin(angle);
    return velocity;
}

olc::vf2d BitmapDynamicSolid::GetPosition(){
    if(!was_updated) return position;
    return position - GetVelocity();
}

void BitmapDynamicSolid::Update(){
    //was_updated = true;
    //velocity = GetVelocity();
    //position += velocity;
    position += velocity;
    angle+=0.1;
    velocity.x = 2.0f; //change vel after movement to avoid wacky inaccurate adding of velocity
    velocity.y = 5.0f * std::sin(angle);
    //std::cout << velocity.y << std::endl;
    timelapse++;
    if(timelapse > 400){velocity *= -1.0f; timelapse = 0;}
}
void BitmapDynamicSolid::Draw(Camera* _camera){
    
}