#include <memory>
#include <wrap_menu.h>
#include <ufo_maths.h>
#include <button.h>
#include "pingu.h"
#include "pingus_level.h"
#include "item_select_menu.h"
#include <nine_patch_theme.h>

ItemSelectMenu::ItemSelectMenu(Vector2f _local_position) : WrapMenu(Vector2f(6.0f,-4.0f), Vector2f(50.0f,200.0f)){
    has_modified_controls = true;
    spacing = 8;
}

void ItemSelectMenu::OnStart(Level* _level){
    WrapMenu::OnStart(_level);

    item_blow_up = [this](Pingu* _pingu){
        if(number_of_bombers > 0){
            
            if(_pingu->item_blow_up()) number_of_bombers--;
            buttons[selected_index]->text = "    x"+std::to_string(number_of_bombers);
        }
    };

    item_build = [this](Pingu* _pingu){
        if(number_of_builders > 0){
            
            if(_pingu->item_build()) number_of_builders--;
            buttons[selected_index]->text = "    x"+std::to_string(number_of_builders);
        }
    };

    item_parachute = [this](Pingu* _pingu){
        if(number_of_parachutes > 0){
            
            if(_pingu->item_parachute()) number_of_parachutes--;
            buttons[selected_index]->text = "    x"+std::to_string(number_of_parachutes);
        }
    };

    item_blocker = [this](Pingu* _pingu){
        
        if(_pingu->what_is_current_state == Pingu::States::BLOCKER){
            _pingu->item_walk();
            return;
        }

        if(number_of_blockers > 0){
            
            if(_pingu->item_block()) number_of_blockers--;
            buttons[selected_index]->text = "   x"+std::to_string(number_of_blockers);
        }
    };

    item_climber = [this](Pingu* _pingu){
        if(number_of_climbers > 0){
            
            if(_pingu->item_climber()) number_of_climbers--;
            buttons[selected_index]->text = "   x"+std::to_string(number_of_climbers);
        }
    };

    item_driller = [this](Pingu* _pingu){
        if(number_of_drillers > 0){
            
            if(_pingu->item_driller()) number_of_drillers--;
            buttons[selected_index]->text = "    x"+std::to_string(number_of_drillers);
        }
    };

    item_jumper = [this](Pingu* _pingu){
        if(number_of_jumpers > 0){
            
            if(_pingu->item_jump()) number_of_jumpers--;
            buttons[selected_index]->text = "    x"+std::to_string(number_of_jumpers);
        }
    };

    if(number_of_bombers > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "    x"+std::to_string(number_of_bombers));
        b->AddChild<WidgetSpriteReference>(
            "bomber_icon",
            Vector2f(4.0f,-3.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(20.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_blow_up);
    }
    if(number_of_builders > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "    x"+std::to_string(number_of_builders));
        b->AddChild<WidgetSpriteReference>(
            "builder_icon",
            Vector2f(4.0f,-3.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(24.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_build);
    }
    if(number_of_parachutes > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "    x"+std::to_string(number_of_parachutes));
        b->AddChild<WidgetSpriteReference>(
            "parachute_icon",
            Vector2f(4.0f,-3.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(24.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_parachute);
    }
    if(number_of_blockers > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "   x"+std::to_string(number_of_blockers));
        b->AddChild<WidgetSpriteReference>(
            "blocker_icon",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(24.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_blocker);
    }
    if(number_of_climbers > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "   x"+std::to_string(number_of_climbers));
        b->AddChild<WidgetSpriteReference>(
            "climber_icon",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(24.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_climber);
    }
    if(number_of_drillers > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "    x"+std::to_string(number_of_drillers));
        b->AddChild<WidgetSpriteReference>(
            "driller_icon",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(24.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_driller);
    }
    if(number_of_jumpers > 0){
        Button* b = AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(68.0f,20.0f), "    x"+std::to_string(number_of_jumpers));
        b->AddChild<WidgetSpriteReference>(
            "jumper_icon",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(24.0f,20.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        items.push_back(item_jumper);
    }

    dynamic_cast<PingusLevel*>(_level)->item_select_menu = this;

    for(auto&& button : buttons){
        button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
        button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
        button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    }

}