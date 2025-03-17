#pragma once

#include <actor.h>
#include <ufo_maths.h>

class Camera;
class SpriteReference;
class Level;
class PingusLevel;
class LevelWidgetSpriteReference;

///@spawn;
class WorldMapLocation : public Actor{
public:
    bool selected = false;

    PingusLevel* level = nullptr;

    SpriteReference* spr = nullptr;
    LevelWidgetSpriteReference* preview_image = nullptr;
    float f_tint = 0.0f;

    ///@export;
    bool unlocked = false;

    ///@export("name");
    std::string name_of_location = "";

    ///@export("path");
    std::string level_path = "";

    ///@export;
    std::string unlocks_other_level = "?";

    ///@export;
    std::string preview = "";

    WorldMapLocation* other_location = nullptr;

    WorldMapLocation(Vector2f _location) : Actor(_location){}

    void OnLevelEnter(Level*);
    void OnStart(Level* _level);
    void OnUpdate();
    void OnDraw(Camera*);

    void Unlock();

};