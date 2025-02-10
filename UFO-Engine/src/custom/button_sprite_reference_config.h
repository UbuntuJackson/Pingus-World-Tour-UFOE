#ifndef BUTTON_SPRITE_REFERENCE_CONFIG_H
#define BUTTON_SPRITE_REFERENCE_CONFIG_H

#include <string>
#include <memory>
#include "../widgets/button.h"
#include "../external/olcPixelGameEngine.h"
class ButtonViewSpriteReferences;

class ButtonSpriteReferenceConfig : public Button{
public:
    int actor_id;
    Button* parent_button;
    ButtonSpriteReferenceConfig(
        int _x,
        int _y,
        int _w,
        int _h,
        std::string _text,
        int _actor_id,
        Button* _parent_button);
    void OnUpdate();
    void OnWidgetDraw();

};

#endif