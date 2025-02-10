#include <memory>
#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../json/json.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "battle_participant.h"
#include "../level/level.h"
#include "tuxtale_battle_hud.h"

BattleParticipant::BattleParticipant(Vector2f _local_position,TuxTaleHud* _tuxtale_hud) : Shape<Rectangle>(Rectangle(Vector2f(0.0f, 0.0f), Vector2f(14.0f, 14.0f)),_local_position, false, Vector2f(-7.0f, -7.0f)),
tuxtale_hud{_tuxtale_hud}{}

void BattleParticipant::OnLevelEnter(Level* _level){
    Shape<Rectangle>::OnLevelEnter(_level);
    original_location = local_position;

    tuxtale_hud->battle_participant_handles.push_back(this);
}

void BattleParticipant::OnRetreat(){
    Vector2f dir = (original_location-GetGlobalPosition()).norm();

    float former_dist = (original_location-GetGlobalPosition()).mag();
    
    velocity += dir * 1000.0f * Engine::Get().GetDeltaTime();
    local_position += velocity * Engine::Get().GetDeltaTime();

    float current_dist = (original_location-GetGlobalPosition()).mag();

    if(former_dist < current_dist){
        local_position = original_location;
        tuxtale_hud->turn++;
    }
}