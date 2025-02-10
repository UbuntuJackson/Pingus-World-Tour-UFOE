#ifndef RECTANGLE_SELECTION
#define RECTANGLE_SELECTION

#include "../external/olcPixelGameEngine.h"
#include "../mouse/mouse.h"
#include "../level/ufo_engine.h"
#include "../shapes/shape.h"

class RectangleSelection : public Shape<Rectangle>{
public:
    RectangleSelection(olc::vf2d _position) : Shape<Rectangle>(Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(0.0f, 0.0f)),_position){

    }

    void HandleUpdate(){

        if(Mouse::GetLeftButton().is_pressed) position = Engine::Get().current_level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());

        if(Mouse::GetLeftButton().is_held){
            shape.size = Engine::Get().current_level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition()) - shape.position;
        }
    }

    Rectangle GetRectangle(){
        Rectangle rect = shape;
        if(shape.size.x < 0.0f){rect.position.x = rect.position.x+rect.size.x; rect.size.x = -rect.size.x;}
        if(shape.size.y < 0.0f){rect.position.y = rect.position.y+rect.size.y; rect.size.y = -rect.size.y;}
    }
};

#endif