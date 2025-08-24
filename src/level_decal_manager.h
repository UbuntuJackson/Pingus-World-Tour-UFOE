#pragma once

#include <actor.h>
#include <level.h>
#include <ufo_maths.h>
#include <olcPixelGameEngine.h>
#include "pingus_level.h"

//This object is used to update decals at the end of the frame upon request. The the OnUpdate function
class LevelDecalManager : public Actor{
public:
    LevelDecalManager() : Actor(Vector2f(0.0f, 0.0f)){

    }

    PingusLevel* level = nullptr;

    void OnLevelEnter(Level* _level){
        level = dynamic_cast<PingusLevel*>(_level);

        //Just make sure this happens at the end of the frame
        SetZIndex(100);
    }

    void OnUpdate(){
        
        if(level->should_update_level_decals){
            olc::Decal* dec = level->asset_manager.GetDecal("solid");
            olc::Decal* dec_mg = nullptr;
            if(level->asset_manager.decals.count("mg")) dec_mg = level->asset_manager.GetDecal("mg");
        
            dec->Update();
            if(dec_mg) dec_mg->Update();
        
            level->should_update_level_decals = false;
        }
    }

};