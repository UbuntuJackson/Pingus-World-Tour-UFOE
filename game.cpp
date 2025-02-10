#include <ufo_engine.h>
#include "src/pingus_level.h"

//#include "UFO-Engine/src/level/level.h"

int main(){

    Engine& ufo = Engine::Get();

    //ufo.GoToLevel("../UFO-Engine/res/engine/default_level.json");
    ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/test_level/test_level_tiled.json");
    ufo.Start();

    return 0;
}