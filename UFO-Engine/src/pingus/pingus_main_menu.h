#pragma once

#include <memory>
#include "../widgets/wrap_menu.h"

class PingusMainMenu : public WrapMenu{
public:
    PingusMainMenu();
    void OnLevelEnter(Level* _level);
};