#pragma once

#include <memory>
#include "../widgets/wrap_menu.h"
#include <ufo_maths.h>

class PingusMainMenu : public WrapMenu{
public:
    PingusMainMenu(Vector2f _local_position);
    void OnLevelEnter(Level* _level);
    void OnUpdate();
};