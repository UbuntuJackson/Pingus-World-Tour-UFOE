#ifndef ONE_DIMENSIONAL_TEXT_BOX
#define ONE_DIMENSIONAL_TEXT_BOX

#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../level/level.h"
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "label.h"
#include "../drawing_system/drawing_system.h"
#include "../external/olcPixelGameEngine.h"

enum WrapMode{
    WRAP_ON_CHAR_MEETS_BORDER,
    WRAP_ON_SPACE
};

class OneDimensionalTextBox : public Label{
public:
    std::string speaker;
    std::vector<std::string> text_queue;
    std::vector<std::string> past_texts_queue;

    OneDimensionalTextBox(
        Vector2f _local_position,
        Vector2f _size,
        std::string _speaker = ""
    ) :
    Label(_local_position, _size),
    speaker{_speaker}
    {
        
    }

    void OnLevelEnter(Level* _level){
        if(text_queue.size() == 0) return;

        if(text_queue.size() > 0){
            text = GetWrappedTextWrapOnSpace(text_queue.back()).text;
            past_texts_queue.push_back(text_queue.back());
            text_queue.pop_back();
        }
    }

    bool Next(){

        Console::Out(text_queue);

        if(text_queue.size() > 0){
            text = GetWrappedTextWrapOnSpace(text_queue.back()).text;
            past_texts_queue.push_back(text_queue.back());
            text_queue.pop_back();
            return true;
        }

        QueueForPurge();
        return false;
    }

    bool Back(){
        if(past_texts_queue.size() > 0){
            text_queue.push_back(past_texts_queue.back());
            past_texts_queue.pop_back();
            text = GetWrappedTextWrapOnSpace(text_queue.back()).text;
            return true;
        }
        return false;
    }

    void OnWidgetDraw(){
        rectangle.position = GetGlobalPosition();

        DrawingSystem::DrawFilled(rectangle, olc::Pixel(0,100,100,240));

        DrawingSystem::DrawString(GetGlobalPosition()+Vector2f(3.0f,3.0f), text, colour, scale);
    }

};

#endif