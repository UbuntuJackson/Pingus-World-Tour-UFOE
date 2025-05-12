#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include "pingu.h"

class PinguSelectionManager : public Actor{
public:
    PingusLevel* level = nullptr;
    
    PinguSelectionManager() : Actor(Vector2f(0.0f,0.0f)){

    }

    void OnLevelEnter(Level* _level){
        level = dynamic_cast<PingusLevel*>(_level);
        SetZIndex(-1);
    }

    void OnUpdate(){
        for(int i = level->pingu_handles_all_pingus.size()-1; i >= 0; i--){
            level->pingu_handles_all_pingus[i]->OnSelectionIteration();
        }
    }
};