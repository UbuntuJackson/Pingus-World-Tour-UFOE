#include <memory>
#include "widget.h"
#include "../shapes/rectangle.h"
#include "../json/json.h"
#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "../drawing_system/drawing_system.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/level.h"

using namespace ufoMaths;

Rectangle
Widget::GetRectangle(){
    return Rectangle(GetGlobalPosition(), rectangle.size);
}

Widget::Widget(Vector2f _local_position, Vector2f _size) :
    Actor(_local_position), rectangle(_local_position, _size){}

void Widget::OnLevelEnter(Level* _level){
    Actor::OnLevelEnter(_level);

}

void Widget::OnUpdate(){
    Actor::OnUpdate();
    
}

void Widget::OnWidgetDraw(){
    if(!theme.non_default_theme) DrawingSystem::Draw(rectangle, olc::BLUE);
    else{
        theme.OnDraw(this);
    }
}