#pragma once

#include "../shapes/shape.h"
#include "../shapes/rectangle.h"
#include "../ufo_maths/ufo_maths.h"

class MovablePillar : public Shape<Rectangle>{
public:
    MovablePillar(Vector2f _local_position) : Shape<Rectangle>(Rectangle(Vector2f(0.0f, 0.0f),Vector2f(16.0f, 16.0f)), _local_position){

    }

    void OnUpdate(){
        Shape<Rectangle>::OnUpdate();
    }
};