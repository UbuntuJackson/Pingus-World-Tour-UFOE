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

ufo::Rectangle
Widget::GetRectangle(){
    return ufo::Rectangle(GetGlobalPosition(), rectangle.size);
}

Widget::Widget(Vector2f _local_position, Vector2f _size) :
    Actor(_local_position), rectangle(_local_position, _size){}

void Widget::OnLevelEnter(Level* _level){
    Actor::OnLevelEnter(_level);

}

WrappedText Widget::GetWrappedTextWrapOnSpace(std::string _text){

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

WrappedText Widget::GetWrappedTextWrapOnCharMeetsBorder(std::string _text){
    int text_width = 8;
    std::string new_text = "";
    int number_of_rows = 0;

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

void Widget::OnUpdate(){
    Actor::OnUpdate();
    
}

void Widget::OnWidgetDraw(){
    if(!visible) return;
    theme->OnDraw(this);
    
}