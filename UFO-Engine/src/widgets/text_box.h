#include <functional>
#include <string>
#include <vector>
#include "../level/level.h"
#include "widget.h"
#include "../ufo_maths/ufo_maths.h"

class OptionData{
    OptionData(
        std::string _text,
        std::function<void(Level* _level)> _event
    ){}
};

class TextBox : public Widget{
public:
    std::string speaker;
    std::string text;
    std::function<void(Level* _level)> event;
    std::vector<OptionData> options;

    TextBox(
        Vector2f _local_position,
        Vector2f _size,
        std::string _speaker,
        std::string _text,
        std::function<void(Level* _level)> _event,
        std::vector<OptionData> _options
    ) :
    Widget(_local_position, _size),
    speaker{_speaker},
    text{_text},
    event{_event},
    options{_options}
    {}
};