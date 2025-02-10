#include <string>
#include "../external/olcPixelGameEngine.h"
#include "../ufo_engine/ufo_engine.h"
#include "button_view_sprite_references.h"
#include "button_sprite_reference_config.h"
#include "../widgets/button.h"
#include "../console/console.h"
#include "../drawing_system/drawing_system.h"

ButtonSpriteReferenceConfig::ButtonSpriteReferenceConfig(
    int _x,
    int _y,
    int _w,
    int _h,
    std::string _text,
    int _actor_id,
    Button* _parent_button) :
    Button(Vector2f(_x, _y),Vector2f(_w, _h), _text),
    actor_id{_actor_id},
    parent_button{_parent_button}
    {
    }
void ButtonSpriteReferenceConfig::OnUpdate(){

    if(IsReleased()){
        Engine::Get().current_level->QueueForPurge(actor_id);
        parent_button->needs_to_refresh = true;
    }
}
void ButtonSpriteReferenceConfig::OnWidgetDraw(){
    rectangle.position = position;
    if(IsHovered()) DrawingSystem::DrawFilled(rectangle, olc::DARK_GREY);
    if(!IsHovered()) DrawingSystem::DrawFilled(rectangle, olc::VERY_DARK_GREY);
    if(IsHeld()) DrawingSystem::DrawFilled(rectangle, olc::Pixel(20.0f, 20.0f, 20.0f));

    Engine::Get().pixel_game_engine.DrawStringDecal(position + olc::vf2d(5.0f, 4.0f), text, olc::WHITE, {2.0f, 2.0f});
}