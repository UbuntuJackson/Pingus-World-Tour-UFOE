#ifndef WIDGET_H
#define WIDGET_H

#include <memory>
#include <vector>
#include "../shapes/rectangle.h"
#include "../external/olcPixelGameEngine.h"
#include "../camera/camera.h"
#include "../json/json.h"
#include "../actor/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "colour_rectangle_theme.h"
#include "../graphics/graphics.h"

class Level;

//Wrapped text contains not only a string, but also the number of times the text
//wraps around.
struct WrappedText{
    std::string text;
    int rows;
};

//The baseclass for all widgets. It is not abstract and can be used as
//a frame for other widgets. You can put for example Buttons and WidgetSpriteReferences on it.

class Widget : public Actor{
public:
    bool visible = true;
//protected:
    ufo::Rectangle rectangle;
//public:
    ufo::Rectangle GetRectangle();

    //All widgets has a default theme. They will have additional themes after need.
    std::unique_ptr<Theme> theme = std::make_unique<ColourRectangleTheme>(Graphics::DARK_CYAN);

    Level* level = nullptr;

    Widget(Vector2f _local_position, Vector2f _size);

    void OnLevelEnter(Level* _level);

    ufo::Rectangle ShapeWithRespectToParentWidgetBorder(const ufo::Rectangle& _r){
        ufo::Rectangle return_rectangle = _r;
        if(parent){
            auto w_parent = dynamic_cast<Widget*>(parent);

            if(w_parent){
                //position is local here
                if(_r.position.y+_r.size.y > w_parent->rectangle.size.y) return_rectangle.size.y = w_parent->rectangle.size.y - _r.position.y;

                if(_r.position.y < 0.0f){
                    return_rectangle.size.y += _r.position.y;

                    return_rectangle.position.y = 0.0f;
                }
            }
        }
        return return_rectangle;
    }
    
    //Wraps when a word would otherwise go outside of the widget that encapsulates it.
    WrappedText GetWrappedTextWrapOnSpace(std::string _text);

    //Wraps the word so that the characters that would otherwise go outside of the widget
    //end up on the next row.
    WrappedText GetWrappedTextWrapOnCharMeetsBorder(std::string _text);

    void OnUpdate();

    void OnWidgetDraw();

};

#endif