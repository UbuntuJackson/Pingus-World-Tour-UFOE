#include <wrap_menu.h>
#include <ufo_maths.h>
#include <button.h>
#include "pingu.h"
#include "pingus_level.h"
#include "item_select_menu.h"

ItemSelectMenu::ItemSelectMenu(Vector2f _local_position) : WrapMenu(Vector2f(0.0f,0.0f), Vector2f(50.0f,20.0f)){

}

void ItemSelectMenu::OnStart(Level* _level){

    std::function<void(Pingu* _pingu)> item_blow_up = [this](Pingu* _pingu){
        _pingu->item_blow_up();
    };

    std::function<void(Pingu* _pingu)> item_build = [this](Pingu* _pingu){
        _pingu->item_build();
    };

    std::function<void(Pingu* _pingu)> item_parachute = [this](Pingu* _pingu){
        _pingu->item_parachute();
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
        AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Parachute : "+std::to_string(number_of_builders));
        items.push_back(item_parachute);
    }

    dynamic_cast<PingusLevel*>(_level)->item_select_menu = this;

    WrapMenu::OnLevelEnter(_level);

}