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

struct WrappedText{
    std::string text;
    int rows;
};

using namespace ufoMaths;

class Widget : public Actor{
public:
    bool visible = true;
    ufo::Rectangle rectangle;
    ufo::Rectangle GetRectangle();

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
    
    WrappedText GetWrappedTextWrapOnSpace(std::string _text){

        int text_width = 8;

        std::vector<std::string> text_as_vector;

        std::string word = "";

        for(int c = 0; c < _text.size(); c++){
            
            if(_text[c] != ' ') word += _text[c];

            if(c == _text.size()-1 || _text[c] == ' ' /*|| _text[c] == '\n'*/){
                text_as_vector.push_back(word);
                word = "";
            }
        }

        std::string new_text = "";

        std::string row = "";

        int number_of_rows = 1;

        for(int i_word = 0; i_word < text_as_vector.size(); i_word++){
            
            for(int c = 0; c < text_as_vector[i_word].size(); c++){
                if(text_as_vector[i_word][c] == '\n'){
                    number_of_rows++;
                }
            }

            if(row.size()+text_as_vector[i_word].size() < int(rectangle.size.x/text_width)){
                row+=text_as_vector[i_word]+" ";
            }
            else{
                number_of_rows++;
                new_text+=(row+"\n");
                row = "";
                row+=text_as_vector[i_word]+" ";
            }
        }
        new_text+=row;
        
        return {new_text, number_of_rows};
    }

    WrappedText GetWrappedTextWrapOnCharMeetsBorder(std::string _text){
        int text_width = 8;
        std::string new_text = "";
        int number_of_rows;

        for(int c = 0; c < _text.size(); c++){
            if(c % int(rectangle.size.x/text_width) == int(rectangle.size.x/text_width)-1){
                new_text+="\n";
                number_of_rows++;
            }
            new_text+=_text[c];

            //Console::Out((c*text_width)%int(rectangle.size.x));
        }

        return {new_text, number_of_rows};
    }

    void OnUpdate();

    void OnWidgetDraw();

};

#endif