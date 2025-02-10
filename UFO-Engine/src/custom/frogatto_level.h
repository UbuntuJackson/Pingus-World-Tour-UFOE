#ifndef FROGATTO_LEVEL
#define FROGATTO_LEVEL

#include "../level/level.h"
#include "../json/json.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"
#include "../sprite_reference/sprite_reference.h"
//#include "../garbage_collected/garbage_collected.h"

class HeartBubble;
class Balloon;
class Circle;
class Ant;
class Edible;
class Camera;
class WaterBody;

class FrogattoLevel : public Level{
public:
    std::vector<Balloon*> balloon_handles;
    std::vector<Shape<Circle>*> coin_handles;
    std::vector<Ant*> enemy_handles;
    std::vector<HeartBubble*> heart_bubble_handles;
    std::vector<Edible*> edible_handles;
    std::vector<WaterBody*> water_body_handles;

    //GarbageCollected<SpriteReference> gc_sprite;
    //GarbageCollected<SpriteReference> gc_sprite2;

    /*void TestPassGarbageCollected(GarbageCollected<SpriteReference> _gc){
        Console::Out("Passed",_gc.Get()->reference_count);
    }*/

    FrogattoLevel();

    void OnUpdate();

    void OnLoadActors(Json& _actor_json);
    void OnLoadLayer(Json& _layer_json);
    void OnResourceLoad();
    void OnHandlePurge();

    void OnBackgroundElementDraw(Camera* _camera);

};

#endif