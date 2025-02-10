#pragma once

#include <memory>
#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../json/json.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "battle_participant.h"

class OneDimensionalTextBox;
class Level;
class TuxTaleLevel;
class Animation;
class TuxTaleHud;

using namespace ufoMaths;

class BattleEnemy : public BattleParticipant{
public:
    Animation* animation;
    BattleEnemy(Vector2f _local_position, TuxTaleHud* _tuxtale_hud);
    void OnLevelEnter(Level* _level);
    void OnUpdate();
    void OnTurn();
    
};