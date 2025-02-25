#include <memory>
#include <wrap_menu.h>
#include <ufo_maths.h>
#include <button.h>
#include "pingu.h"
#include "pingus_level.h"
#include "item_select_menu.h"
#include <nine_patch_theme.h>

ItemSelectMenu::ItemSelectMenu(Vector2f _local_position) : WrapMenu(Vector2f(4.0f,4.0f), Vector2f(50.0f,20.0f)){
    has_modified_controls = true;
    spacing = 8;
}

void ItemSelectMenu::OnStart(Level* _level){

    item_blow_up = [this](Pingu* _pingu){
        if(number_of_bombers > 0){
            
            if(_pingu->item_blow_up()) number_of_bombers--;
            buttons[selected_index]->text = "Bomber : "+std::to_string(number_of_bombers);
        }
    };

    item_build = [this](Pingu* _pingu){
        if(number_of_builders > 0){
            
            if(_pingu->item_build()) number_of_builders--;
            buttons[selected_index]->text = "Builder : "+std::to_string(number_of_builders);
        }
    };

    item_parachute = [this](Pingu* _pingu){
        if(number_of_parachutes > 0){
            
            if(_pingu->item_parachute()) number_of_parachutes--;
            buttons[selected_index]->text = "Parachute : "+std::to_string(number_of_parachutes);
        }
    };

    item_blocker = [this](Pingu* _pingu){
        if(number_of_blockers > 0){
            
            if(_pingu->item_block()) number_of_blockers--;
            buttons[selected_index]->text = "Blocker : "+std::to_string(number_of_blockers);
        }
    };

    item_blocker = [this](Pingu* _pingu){
        if(number_of_climbers > 0){
            
            if(_pingu->item_climber()) number_of_climbers--;
            buttons[selected_index]->text = "Blocker : "+std::to_string(number_of_climbers);
        }
    };

    if(number_of_bombers > 0){
        AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Bomber : "+std::to_string(number_of_bombers));
        items.push_back(item_blow_up);
    }
    if(number_of_builders > 0){
        AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Builder : "+std::to_string(number_of_builders));
        items.push_back(item_build);
    }
    if(number_of_parachutes > 0){
        AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Parachute : "+std::to_string(number_of_parachutes));
        items.push_back(item_parachute);
    }
    if(number_of_blockers > 0){
        AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Blocker : "+std::to_string(number_of_blockers));
        items.push_back(item_blocker);
    }
    if(number_of_climbers > 0){
        AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Climber : "+std::to_string(number_of_climbers));
        items.push_back(item_blocker);
    }

    dynamic_cast<PingusLevel*>(_level)->item_select_menu = this;

    WrapMenu::OnLevelEnter(_level);

    for(auto&& button : buttons){
        button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
        button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    }

}