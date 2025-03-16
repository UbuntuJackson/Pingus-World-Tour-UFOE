#pragma once

#include <actor.h>
#include <ufo_maths.h>

class Camera;

///@spawn;
class WorldMapLocation : public Actor{
public:
    bool selected = false;

    ///@export("name");
    std::string name_of_location = "";

    ///@export("path");
    std::string level_path = "";

    WorldMapLocation(Vector2f _location) : Actor(_location){}

    void OnLevelEnter(Level*);
    void OnDraw(Camera*);

};