#include "menu_maker.h"
#include "new_widget.h"
#include "../json/json.h"

void MenuMaker::LoadWidgets(Json& _json_from_scene_file){
    widgets.clear();
    widget_jsons.clear();

    Json::ArrayForEach(Json(), _json_from_scene_file.GetObject("widgets"), [&](Json _object, Json _array){
        widgets.push_back(NewWidget(&_object));
    });
}