#pragma once
#include <string>
#include <memory>
#include <ufo_maths.h>
#include <button.h>
#include <wrap_menu.h>

class NewGameMenu : public WrapMenu{
public:
    NewGameMenu(Vector2f _local_position);
    void OnLevelEnter(Level* _level);
    void OnUpdate();

    void WriteNewSave(std::string _path, std::string _profile_name);
};