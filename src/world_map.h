#include <string>
#include <actor.h>
#include <ufo_maths.h>
#include <sprite_reference.h>

class Level;
class PingusLevel;
class WidgetSpriteReference;

/// @spawn;
class WorldMap : public Actor{
public:

    int currently_selected_location = 0;

    std::string last_played_level = "?";
    int last_played_level_rank = 3;

    WidgetSpriteReference* backdrop = nullptr;
    float backdrop_velocity = 0.0f;
    float backdrop_acceleration = 5000.0f;

    PingusLevel* level = nullptr;
    WorldMap(Vector2f _) : Actor(Vector2f(0.0f,0.0f)){}

    void OnLevelEnter(Level* _level);

    void OnStart(Level* _level);
    void OnUpdate();

};