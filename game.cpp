#include <ufo_engine.h>
#include "src/pingus_level.h"
#include <ufo_maths.h>

//#include "UFO-Engine/src/level/level.h"

int main(){

    int result = ufoMaths::Wrapi(-1,0,2);
    int result2 = ufoMaths::Wrap(-1,0,1);
    //Console::Out(result,result2,-1%3);
    //return 0;

    Engine& ufo = Engine::Get();

    //ufo.GoToLevel("../UFO-Engine/res/engine/default_level.json");
    ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/builder_test_level/test_level_builder_tiled.json");
    ufo.Start();

    return 0;
}