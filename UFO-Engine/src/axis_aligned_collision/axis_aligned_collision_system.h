#ifndef AXIS_ALIGNED_COLLISION_SYSTEM
#define AXIS_ALIGNED_COLLISION_SYSTEM

#include "../shapes/rectangle.h"
#include "axis_aligned_collision_body.h"
#include "../ufo_maths/ufo_maths.h"
#include "../drawing_system/drawing_system.h"
#include "../json/json.h"

class AxisAlignedCollisionSystem{
public:
    AxisAlignedCollisionSystem() = default;
    void LoadTiles(Json& _json);

    int level_tile_width;
    int collision_tile_width;
    int collision_tile_height;
    std::vector<int> collision_tiles;

    std::vector<ufo::Rectangle> active_tiles;

    void Update(AxisAlignedCollisionBody* _collisionbody);

    void Draw(Camera* _camera);
};

#endif