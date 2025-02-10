#pragma once
#include <shape.h>
#include <rectangle.h>
#include <level.h>
#include <ufo_maths.h>

class Goal : public Shape<Rectangle>{
public:
    int number_of_pingus = 0;
    Goal(Vector2f _local_position) : Shape<Rectangle>(Rectangle(Vector2f(0.0f, 0.0f),Vector2f(16.0f,16.0f)),_local_position){

    }
    
};