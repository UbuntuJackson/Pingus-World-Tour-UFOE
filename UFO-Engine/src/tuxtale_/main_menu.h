#ifndef MAIN_MENU
#define MAIN_MENU
#include <memory>
#include <functional>
#include "../widgets/button.h"
#include "../widgets/wrap_menu.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../level/level.h"
#include "tuxtale_level.h"

std::unique_ptr<Level> MainMenu();

#endif