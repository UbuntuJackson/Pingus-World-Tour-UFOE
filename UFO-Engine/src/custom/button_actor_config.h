#ifndef BUTTON_ACTOR_CONFIG_H
#define BUTTON_ACTOR_CONFIG_H

#include <string>
#include <memory>
#include "../widgets/button.h"
#include "../external/olcPixelGameEngine.h"
class ButtonViewCameras;

class ButtonActorConfig : public Button{
public:
    int actor_id;
    Button* parent_button;
    ButtonActorConfig(
        int _x,
        int _y,
        int _w,
        int _h,
        std::string _text,
        int _actor_id,
        Button* _parent_button);

    void OnLevelEnter(Level* _level);
    void OnUpdate();
    void OnWidgetDraw();

};

#endif