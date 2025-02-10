#pragma once
#include <stdexcept>
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../shapes/raw_shape_base.h"
#include "../camera/camera.h"

class RawShapeBase;

class ModifiableShape : public Actor{
private:
    RawShapeBase* raw_shape_base = nullptr;
    Vector2f offset;

public:
    bool always_visible = false;

    ModifiableShape(Vector2f _local_position, RawShapeBase* _shape, bool _always_visible = false){

    }

    RawShapeBase* GetShape(){ //Get the shape in global space
        if(raw_shape_base == nullptr) throw std::runtime_error("ModifiableShape::GetShape() raw_shape_base is nullptr");
        raw_shape_base->position = GetGlobalPosition()+offset;
        return raw_shape_base;
    }
    void OnDebugDraw(Camera* _camera){
        raw_shape_base->Draw(_camera);
    }

};