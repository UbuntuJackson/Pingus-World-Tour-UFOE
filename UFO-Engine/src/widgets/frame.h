#include "widget.h"

class VerticalScrollContainer : public Widget{
public:

    void OnUpdate(){
        
    }

    void GetElementAtMousePosition(olc::vf2d _mouse_position_on_screen){

    }

    void OnWidgetDraw(){
        float total_height_of_children = 0.0f;
        
        for(auto&& child : children){
            total_height_of_children+=child.height;
        }
        
        if(height > total_height_of_children){
            //Activate scrollbar
        }
    }
};