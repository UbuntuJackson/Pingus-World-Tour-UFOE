#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include <memory>
#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "../json/json.h"

class SpawnPoint : public Actor{
public:
    std::string name;
    SpawnPoint(olc::vf2d _position, std::string _name = "");
    static std::unique_ptr<SpawnPoint> Load(Json& _json);
};

#endif