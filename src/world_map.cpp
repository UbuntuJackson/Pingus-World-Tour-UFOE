#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>
#include "world_map.h"

void WorldMap::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level); 
    level->is_menu = true;
    
}