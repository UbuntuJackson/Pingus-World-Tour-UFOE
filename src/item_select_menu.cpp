#include <wrap_menu.h>
#include <ufo_maths.h>
#include <button.h>
#include "pingu.h"
#include "pingus_level.h"
#include "item_select_menu.h"

ItemSelectMenu::ItemSelectMenu(Vector2f _local_position) : WrapMenu(Vector2f(0.0f,0.0f), Vector2f(50.0f,20.0f)){
    has_modified_controls = true;
}

void ItemSelectMenu::OnStart(Level* _level){

    std::function<void(Pingu* _pingu)> item_blow_up = [this](Pingu* _pingu){
        if(number_of_bombers > 0){
            _pingu->item_blow_up();
            number_of_bombers--;
            buttons[selected_index]->text = "Bomber : "+std::to_string(number_of_bombers);
        }
    };

    std::function<void(Pingu* _pingu)> item_build = [this](Pingu* _pingu){
        if(number_of_builders > 0){
            _pingu->item_build();
            number_of_builders--;
            buttons[selected_index]->text = "Builder : "+std::to_string(number_of_builders);
        }
    };

    std::function<void(Pingu* _pingu)> item_parachute = [this](Pingu* _pingu){
        if(number_of_parachutes > 0){
            _pingu->item_parachute();
            number_of_parachutes--;
            buttons[selected_index]->text = "Parachute : "+std::to_string(number_of_parachutes);
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

    dynamic_cast<PingusLevel*>(_level)->item_select_menu = this;

    WrapMenu::OnLevelEnter(_level);

}