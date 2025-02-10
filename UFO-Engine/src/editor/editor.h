#ifndef EDITOR_H
#define EDITOR_H

#include "../level/actor.h"
#include "../level/ufo_engine.h"
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../mouse/mouse.h"

class Editor{
public:

    virtual void OnUpdate(){
        Level* current_level = Engine::Get().current_level.get();

        for(const auto& actor : current_level->actors){
            if(ufoMaths::RectangleVsPoint(current_level->GetActiveCamera()->Transform(actor->editor_selection_shape),Mouse::Get().GetPosition())){
                if(Mouse::Get().GetLeftButton().is_pressed){
                    current_level->NewActor(actor->GetConfigurationWidget());
                }
            }
        }
    }

};

#endif