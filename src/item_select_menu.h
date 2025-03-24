#pragma once
#include <wrap_menu.h>

class Pingu;

/// @spawn;
class ItemSelectMenu : public WrapMenu{
public:
    /// @export("bomber");
    int number_of_bombers = 0;

    /// @export("builder");
    int number_of_builders = 0;

    /// @export("parachute");
    int number_of_parachutes = 0;

    /// @export("blocker");
    int number_of_blockers = 0;
    
    /// @export("climber");
    int number_of_climbers = 0;

    /// @export("driller");
    int number_of_drillers = 0;

    std::vector<std::function<void(Pingu* _pingu)>> items;

    std::function<void(Pingu* _pingu)> item_blow_up;

    std::function<void(Pingu* _pingu)> item_build;

    std::function<void(Pingu* _pingu)> item_parachute;

    std::function<void(Pingu* _pingu)> item_blocker;

    std::function<void(Pingu* _pingu)> item_climber;

    std::function<void(Pingu* _pingu)> item_driller;
    
    ItemSelectMenu(Vector2f _local_position);

    void OnStart(Level* _level);
};