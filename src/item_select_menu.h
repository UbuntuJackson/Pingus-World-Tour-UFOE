#pragma once
#include <wrap_menu.h>
#include <ufo_maths.h>
#include <button.h>

class ItemSelectMenu : public WrapMenu{
public:
    int number_of_bombers = 0;
    int number_of_builders = 0;

    ItemSelectMenu(Vector2f _local_position) : WrapMenu(Vector2f(0.0f,0.0f), Vector2f(50.0f,20.0f)){

    }

    void OnStart(Level* _level){

        if(number_of_bombers > 0) AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Bomber : "+std::to_string(number_of_bombers));
        if(number_of_builders > 0) AddChild<Button>(Vector2f(0.0f, 0.0f), Vector2f(120.0f,20.0f), "Builder : "+std::to_string(number_of_builders));

        WrapMenu::OnLevelEnter(_level);

    }
};