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

class BattlePlayer : public BattleParticipant{
public:
    Vector2f velocity = {0.0f, 0.0f};

    bool ready = false;
    int attack_id = -1;
    bool can_move = false;
    bool is_on_ground = false;
    float friction_x = 300.0f;
    float acceleration_x = 360.0f;
    float gravity = 200.0f;
    float initial_jump_speed = -131.0f;
    Animation* animation;
    Animation* sword_anim = nullptr;
    
    BattlePlayer(Vector2f _local_position,TuxTaleHud* _tuxtale_hud);
    void OnLevelEnter(Level* _level);
    void OnUpdate();
    void OnTurn();
    
};