/*
    --- This file is belong to the UFO class-collection. Copyright: UbuntuJackson (2024) ---
*/

#ifndef MENU_MAKER_H
#define MENU_MAKER_H

#include <memory>
#include "../mouse/mouse.h"
#include "../camera/camera.h"
#include "widget_bounds.h"
#include "../json/json.h"
#include "../external/olcPixelGameEngine.h"

enum Actions{
    DRAG,
    DROP,
    GRAB
};

enum Tools{
    WIDGET_BOUNDS,
    BUTTON
};

class MenuMaker{
public:
    int selected_tool;
    std::vector<std::unique_ptr<WidgetBounds>> widgets;
    WidgetBounds selected_widget;
    bool is_anything_selected = false;

    void LoadWidgets(Json& _json_from_scene_file);

    void Update(Camera* _camera){
        int action = GetAction();

        switch(selected_tool){
            case Tools::WIDGET_BOUNDS:
                UpdateWidgetBoundsTool(action, _camera);
                break;
        }
    }

    void UpdateWidgetBoundsTool(int _action, Camera* _camera){

        switch(_action){
            case Actions::GRAB:
                {
                    widget_jsons.push_back(Json::New());
                    olc::vf2d position = _camera->TransformScreenToWorld(Mouse::GetPosition()); //need to calculate the relative position though
                    widget_jsons.back().PushToObject("x", position.x); //json returned by back() should be inside parent.
                    widget_jsons.back().PushToObject("y", position.y);
                    break;
                }
            case Actions::DROP:
                {
                    olc::vf2d position;
                    position.x = (float)widget_jsons.back().GetAsDouble("x");
                    position.y = (float)widget_jsons.back().GetAsDouble("y");
                    olc::vf2d size = _camera->TransformScreenToWorld(Mouse::GetPosition()) - position;
                    widget_jsons.back().PushToObject("width", size.x);
                    widget_jsons.back().PushToObject("height", size.y);
                    break;
                }
        }

    }

    int GetAction(){
        if(Mouse::GetLeftButton().is_pressed) return Actions::GRAB;
        if(Mouse::GetLeftButton().is_held) return Actions::DRAG;
        if(Mouse::GetLeftButton().is_released) return Actions::DROP;
    }

};

#endif