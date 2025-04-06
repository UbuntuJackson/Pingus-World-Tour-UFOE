#pragma once
#include <string>
#include <memory>
#include <ufo_maths.h>
#include <button.h>
#include <wrap_menu.h>

class Level;

class LoadGameMenu : public WrapMenu{
public:
    Level* level = nullptr;
    LoadGameMenu(Vector2f _local_position);
    void OnLevelEnter(Level* _level);
    void OnStart(Level* _level);
    void OnUpdate();

    void LoadSave(std::string _path, std::string _profile_name);
};