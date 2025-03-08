#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>

class WorldMap : public Actor{
public:

    int currently_selected_location = 0;

    PingusLevel* level = nullptr;
    WorldMap(Vector2f _) : Actor(Vector2f(0.0f,0.0f)){}

    void OnLevelEnter(Level* _level){
        level = dynamic_cast<PingusLevel*>(_level); 
        level->is_menu = true;
        
    }

    

};