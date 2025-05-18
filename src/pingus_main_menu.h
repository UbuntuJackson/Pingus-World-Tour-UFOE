#pragma once

#include <memory>
#include "../widgets/wrap_menu.h"
#include <ufo_maths.h>

class Level;

/// @spawn;
class PingusMainMenu : public WrapMenu{
public:
    Level* level = nullptr;
    PingusMainMenu(Vector2f _local_position);
    void OnLevelEnter(Level* _level);
    void OnStart(Level* _level);
    void OnUpdate();
};