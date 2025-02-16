#include <ufo_engine.h>
#include "src/pingus_level.h"
#include <ufo_maths.h>

//#include "UFO-Engine/src/level/level.h"

int main(){

    /*for(int i = -10; i < 10; i++){
        Console::Out(ufoMaths::Wrapi(i, 0, 4));
    }

    Console::Out("---");

    for(int i = -10; i < 10; i++){
        Console::Out(ufoMaths::Wrapi(i, -4, 0));
    }

    for(int i = -10; i < 10; i++){
        Console::Out(ufoMaths::Wrapi(i, 0, 1));
    }
    
    return 0;*/

    Engine& ufo = Engine::Get();

    //ufo.GoToLevel("../UFO-Engine/res/engine/default_level.json");
    //ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/builder_test_level/test_level_builder_tiled.json");
    //ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/test_level/test_level_tiled.json");
    ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/parachute_test_level/parachute_test_level.json");
    ufo.Start();

    return 0;
}