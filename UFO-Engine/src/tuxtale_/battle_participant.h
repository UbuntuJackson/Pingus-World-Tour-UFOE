#pragma once

#include <memory>
#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../json/json.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"

class TuxTaleHud;
class BattleParticipant : public Shape<Rectangle>{
public:
    Vector2f original_location;
    Vector2f velocity;
    int state = 0;
    const static int TURN = 0;
    const static int RETREAT = 1;
    TuxTaleHud* tuxtale_hud = nullptr;
    BattleParticipant(Vector2f _local_position,TuxTaleHud* _tuxtale_hud);
    void OnLevelEnter(Level* _level);
    virtual void OnTurn(){};

    void OnRetreat();
};