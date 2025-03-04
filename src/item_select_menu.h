#pragma once
#include <wrap_menu.h>

class Pingu;

class ItemSelectMenu : public WrapMenu{
public:
    int number_of_bombers = 0;
    int number_of_builders = 0;
    int number_of_parachutes = 0;

    std::vector<std::function<void(Pingu* _pingu)>> items;

    std::function<void(Pingu* _pingu)> item_blow_up;

    std::function<void(Pingu* _pingu)> item_build;

    std::function<void(Pingu* _pingu)> item_parachute;
    
    ItemSelectMenu(Vector2f _local_position);

    void OnStart(Level* _level);
};