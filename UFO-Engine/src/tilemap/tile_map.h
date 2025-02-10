#pragma once

#include "../ufo_maths/ufo_maths.h"
#include "../actor/actor.h"
#include "../level/level.h"
#include "../json/json.h"
using namespace ufoMaths;
class Camera;

class TileMap : public Actor{
public:
    
    std::vector<int> tilemap_data;
    Level* level;

    void OnLevelEnter(Level *_level){
        Actor::OnLevelEnter(_level);
        level = _level;

    }

    static std::unique_ptr<TileMap> Load(Json& _layer);

    int GetTileID_AtLevelPosition(Vector2f _position);

    void OnDraw(Camera* _camera);
};