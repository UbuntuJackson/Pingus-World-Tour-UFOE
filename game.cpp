#include <ufo_engine.h>
#include "src/pingus_level.h"
#include <ufo_maths.h>
#include <json_variant.h>
#include <json_dictionary.h>

//#include "UFO-Engine/src/level/level.h"

int main(){

    JsonDictionary j = JsonVariant::Read("../res/json_test_file.json");

    int i = j.Get("x").AsInt();
    
    for(auto&& [k,v] : j.Iterable()){
        if(v->IsFloat()){
            Console::Out("Integer k =",v->AsFloat());
        }

        if(v->IsString()){
            Console::Out("String k =",v->AsString());
        }
    }

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
    //ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/skyscrapers/skyscrapers.json");
    ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/title_screen/title_screen.json");
    ufo.Start();

    return 0;
}