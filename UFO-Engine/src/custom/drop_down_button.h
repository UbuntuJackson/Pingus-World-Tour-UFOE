#ifndef DROP_DOWN_BUTTON
#define DROP_DOWN_BUTTON
#include <string>
#include <memory>
#include "../widgets/button.h"
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"
#include "../drawing_system/drawing_system.h"
#include "../external/olcPixelGameEngine.h"

class DropDownButton : public Button{
public:
    std::unique_ptr<SpriteReference> icon;
    bool is_opened = false;
    DropDownButton(
        int _x,
        int _y,
        int _w,
        int _h) :
        Button(Vector2f(_x, _y),Vector2f(_w, _h))
        {
            text = "Placeholder text";
            
            icon = std::make_unique<SpriteReference>(
                    "character",
                    olc::vf2d(0.0f, 0.0f),
                    olc::vf2d(0.0f, 0.0f),
                    olc::vf2d(32.0f, 32.0f),
                    olc::vf2d(1.0f, 1.0f), 0.0f, 0);
        }

    virtual void CreateButtons(){
        
    }

    void OnUpdate(){
        Button::OnUpdate();

        auto& level = Engine::Get().current_level;

        if(refreshed){
            CreateButtons();
            refreshed = false;
        }

        if(IsReleased()){
            if(!is_opened){
                is_opened = true;
                CreateButtons();
            }
            else{
                for(const auto& i : children) Engine::Get().current_level->QueueForPurge(i->id);
                is_opened = false;
            }
        }

        if(needs_to_refresh){
            for(const auto& i : children){
                level->QueueForPurge(i->id);
            }
            refreshed = true;
            needs_to_refresh = false;
        }
    }

    void OnWidgetDraw(){
        if(IsHovered()) DrawingSystem::DrawFilled(rectangle, olc::CYAN);
        if(!IsHovered()) DrawingSystem::DrawFilled(rectangle, olc::DARK_CYAN);
        if(IsHeld()) DrawingSystem::DrawFilled(rectangle, olc::VERY_DARK_CYAN);
        icon->position = local_position + olc::vf2d(3.0f, 0.0f); 
        DrawingSystem::Draw(*icon.get());
        Engine::Get().pixel_game_engine.DrawStringDecal(icon->position+olc::vf2d(40.0f,8.0f), text, olc::WHITE, {1.0f, 2.0f});
    }
};

#endif