#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include "pingu.h"
#include <olcPixelGameEngine.h>

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
        //Console::PrintLine("PinguSelectionManager");
        for(int i = level->pingu_handles_all_pingus.size()-1; i >= 0; i--){
            level->pingu_handles_all_pingus[i]->OnSelectionIteration();
        }

        //An attempt to do all the is_already_overlapping_blue checks beforehand so that when a pingu moves or is moved by a MovingSolid
        //it is already checked if it was in a blue collision zone before changing position. It did not work out for unknown reasons.
        /*for(int i = level->pingu_handles_all_pingus.size()-1; i >= 0; i--){
            Pingu* pingu = level->pingu_handles_all_pingus[i];
            pingu->is_already_overlapping_blue = pingu->is_already_overlapping_blue = pingu->IsOverlappingFeet(pingu->local_position,olc::BLUE);
        }*/
    }
};