#ifndef BUTTON_VIEW_ACTORS_H
#define BUTTON_VIEW_ACTORS_H
#include <string>
#include <memory>
#include "../widgets/button.h"
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"
#include "../drawing_system/drawing_system.h"
#include "../external/olcPixelGameEngine.h"
#include "button_actor_config.h"
#include "../sprite_reference/sprite_reference.h"
#include "drop_down_button.h"

class ButtonViewActors : public DropDownButton{
public:
    ButtonViewActors(
        int _x,
        int _y,
        int _w,
        int _h) :
        DropDownButton(_x, _y, _w, _h)
        {
            text = "Collision-Bodies";
            
            icon = std::make_unique<SpriteReference>(
                    "character",
                    olc::vf2d(0.0f, 0.0f),
                    olc::vf2d(0.0f, 0.0f),
                    olc::vf2d(32.0f, 32.0f),
                    olc::vf2d(1.0f, 1.0f), 0.0f, 0);
        }

    void CreateButtons(){
        auto& level = Engine::Get().current_level;
        for(int i = 0; i < level->collision_body_handles.size(); i++){
            AddChild(
                std::make_unique<ButtonActorConfig>(
                    0.0f,
                    0.0f+rectangle.size.y+i*rectangle.size.y,
                    rectangle.size.x,
                    rectangle.size.y,
                    "#" + std::to_string(level->collision_body_handles[i]->id),
                    level->collision_body_handles[i]->id,
                    this)
            );
        }
    }
};

#endif