#pragma once

#include <actor.h>
#include <ufo_maths.h>

class Camera;
class SpriteReference;
class Level;
class PingusLevel;

///@spawn;
class WorldMapLocation : public Actor{
public:
    bool selected = false;

    PingusLevel* level = nullptr;

    SpriteReference* spr = nullptr;

    ///@export;
    bool unlocked = false;

    ///@export("name");
    std::string name_of_location = "";

    ///@export("path");
    std::string level_path = "";

    ///@export;
    std::string unlocks_other_level = "";

    WorldMapLocation(Vector2f _location) : Actor(_location){}

    void OnLevelEnter(Level*);
    void OnStart(Level* _level);
    void OnDraw(Camera*);

    void Unlock();

};