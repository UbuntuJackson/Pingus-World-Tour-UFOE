#include "../creation_data/creation_data.h"
#include "../collider.h"
#include "../external/olcPixelGameEngine.h"

namespace ufoStandard{

CreationData<Collider<Rectangle>> NewColliderRectangle(Scene* _scene, olc::vf2d* _velocity, float _x, float _y, float _width, float _height){
    auto creation_data = _scene->AddActorToMap<Collider<Rectangle>>(_scene->colliders, _velocity);
    return creation_data;
}

CreationData<Collider<Rectangle>> NewColliderCircle(Scene* _scene, olc::vf2d* _velocity, float _x, float _y, float _radius){
    auto creation_data = _scene->AddActorToMap<Collider<Rectangle>>(_scene->colliders, _velocity);
    return creation_data;
}

}