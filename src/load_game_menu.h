#pragma once
#include <string>
#include <memory>
#include <ufo_maths.h>
#include <button.h>
#include <wrap_menu.h>

class LoadGameMenu : public WrapMenu{
public:
    LoadGameMenu(Vector2f _local_position);
    void OnLevelEnter(Level* _level);
    void OnUpdate();

    void LoadSave(std::string _path, std::string _profile_name);
};