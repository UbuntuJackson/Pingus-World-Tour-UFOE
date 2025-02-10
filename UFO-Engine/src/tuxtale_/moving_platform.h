#pragma once
#include <memory>
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
using namespace ufoMaths;

class MovingPlatform : public Actor{
public:
    std::vector<int> tile_data;
    int width_in_tiles = 5;
    int platform_height_in_tiles = 5;
    MovingPlatform(Vector2f _local_position) : Actor(_local_position){

    }

    static std::unique_ptr<MovingPlatform> Load(){
        return std::make_unique<MovingPlatform>(Vector2f(0.0f, 0.0f));
    }

    void OnDebugDraw(Camera* _camera){
        
    }
};