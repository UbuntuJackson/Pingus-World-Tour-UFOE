#ifndef DERIVED_SHAPE
#define DERIVED_SHAPE
#include <string>
#include "../shapes/shape.h"
#include "../shapes/rectangle.h"
#include "trigger.h"
#include "level_transition_effect.h"

class LevelTrigger : public Trigger{
public:
    std::string path;
    int tiled_object_id;
    std::string spawn_point_name;
    bool activated = false;
    //bool hit = false;
    LevelTrigger(ufo::Rectangle _rectangle, std::string _path, int _tiled_object_id, std::string _spawn_point_name = "") :
        Trigger(_rectangle),
        path{_path}, tiled_object_id{_tiled_object_id},
        spawn_point_name{_spawn_point_name}{}
    
    void OnHit(Level* _level){
        if(!activated){
            _level->NewActor(std::make_unique<LevelTransitionEffect>(path));
            _level->hit_level_triggers.push_back(this);
            activated = true;
        }
    }
};

#endif