#include <memory>
#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../json/json.h"
#include "../animation/animation.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "battle_enemy.h"
#include "tuxtale_level.h"
#include "../level/level.h"
#include "battle_participant.h"
#include "tuxtale_battle_hud.h"

BattleEnemy::BattleEnemy(Vector2f _local_position, TuxTaleHud* _tuxtale_hud) :
BattleParticipant(_local_position, _tuxtale_hud)
{

}
void BattleEnemy::OnLevelEnter(Level* _level){
    BattleParticipant::OnLevelEnter(_level);

    auto u_animation = std::make_unique<Animation>(Vector2f(0.0f, 0.0f));

    u_animation->AddAnimationState(AnimatedSpriteReference(
        "grass_crawler_battle",
        Vector2f(0.0f, 0.0f),
        Vector2f(16.0f, 12.5f),
        Vector2f(32.0f, 25.0f),
        Vector2f(1.0f, 1.0f),
        0,
        0,
        5.0f
    ));

    animation = u_animation.get();

    u_animation->SetAnimation("grass_crawler_battle");

    AddChild(std::move(u_animation));
}
void BattleEnemy::OnUpdate(){
    Shape<Rectangle>::OnUpdate();

    if(local_position.y > 150.0f) local_position.y = 150.0f;
}

void BattleEnemy::OnTurn(){
    //level->NewActor<Spit>(local_position);
}