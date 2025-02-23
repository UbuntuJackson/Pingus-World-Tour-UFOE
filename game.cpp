#include <ufo_engine.h>
#include "src/pingus_level.h"
#include <ufo_maths.h>
#include <json_variant.h>
#include <json_variant_test.h>

int main(){

    Engine& ufo = Engine::Get();

    ufo.GoToLevel(std::make_unique<PingusLevel>(), "../res/map/title_screen/title_screen.json");
    ufo.Start();

    return 0;
}