#ifndef WRAP_MENU_H
#define WRAP_MENU_H

#include <memory>
#include <vector>
#include "../mouse/mouse.h"
#include "../widgets/widget.h"
#include "../ufo_maths/ufo_maths.h"
#include "../widgets/button.h"
#include "../level/level.h"
#include "../drawing_system/drawing_system.h"
#include "../external/olcPixelGameEngine.h"
#include "../keyboard/single_keyboard.h"

using namespace ufoMaths;

class WrapMenu : public Widget{
public:
    std::vector<Button*> buttons;

    Vector2f original_position;

    int selected_index = 0;
    int spacing = 1;

    bool has_modified_controls = false;

    WrapMenu(Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){

    }

    void OnLevelEnter(Level* _level){
    
        for(const auto& child : new_children_queue){
            Button* b = dynamic_cast<Button*>(child.get());
            if(b != nullptr) buttons.push_back(b);
        }
    
        float total_height = 0.0f;

        for(const auto& button : buttons){
            button->local_position.y = total_height;
            total_height+=button->rectangle.size.y;
            total_height+=spacing;
        }

        rectangle.size.y = total_height;
        rectangle.size.x = 0.0f;

        original_position = local_position;

    }

    void ControlWithKeys(bool _up, bool _down, bool _select){

        if(_up){
            selected_index--;
            selected_index = ufoMaths::Wrapi(selected_index, 0, int(buttons.size()));

            /*if(buttons[selected_index]->GetGlobalPosition().y < 0.0f){
                local_position.y=buttons[selected_index]->GetGlobalPosition().y;
            }
            else{
                local_position.y-=buttons[selected_index]->GetGlobalPosition().y;
            }*/
        }
        if(_down){
            selected_index++;

            if(buttons[ufoMaths::Wrapi(selected_index, 0, int(buttons.size()))]->GetGlobalPosition().y > Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y){
                local_position.y-=(buttons[ufoMaths::Wrapi(selected_index, 0, int(buttons.size()))]->rectangle.size.y+spacing);
            }
            if(selected_index == buttons.size()){
                local_position.y = original_position.y;
            }

            selected_index = ufoMaths::Wrapi(selected_index, 0, int(buttons.size()));

        }
        if(_select){
            buttons[selected_index]->on_pressed(this, buttons[selected_index]);
        }
    }

    void ControlWithMouse(){
        for(int index = 0; index < buttons.size(); index++){
            if(buttons[index]->IsPressed()){
                selected_index = index;
                buttons[index]->on_pressed(this, buttons[index]); 
            }
        }
    }

    void OnUpdate(){

        for(auto&& button : buttons){
            button->is_selected = false;    
        }

        buttons[selected_index]->is_selected = true;
        //Engine::Get().current_level->QueueForPurge(id);
        if(has_modified_controls) return;

        if(SingleKeyboard::Get().GetKey(olc::UP).is_pressed){
            selected_index--;
            selected_index = ufoMaths::Wrapi(selected_index, 0, int(buttons.size()));
            //Console::Out("WrapMenu::selected_index=",selected_index);
        }
        if(SingleKeyboard::Get().GetKey(olc::DOWN).is_pressed){
            selected_index++;

            if(buttons[ufoMaths::Wrapi(selected_index, 0, int(buttons.size()))]->GetGlobalPosition().y > Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y){
                local_position.y-=(buttons[ufoMaths::Wrapi(selected_index, 0, int(buttons.size()))]->rectangle.size.y+spacing);
            }
            if(selected_index == buttons.size()){
                local_position.y = original_position.y;
            }

            selected_index = ufoMaths::Wrapi(selected_index, 0, int(buttons.size()));

        }
        if(SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed){
            buttons[selected_index]->on_pressed(this, buttons[selected_index]);
        }
    }

    void OnPurge(Level *_level){
        
    }

    void OnWidgetDraw(){
        DrawingSystem::DrawFilled(rectangle, olc::Pixel(100,100,100,100));
        Engine::Get().pixel_game_engine.DrawStringDecal(buttons[selected_index]->GetGlobalPosition()-Vector2f(16.0f,0.0f),">");
    }

};

#endif