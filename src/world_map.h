#include <string>
#include <widget.h>
#include <ufo_maths.h>
#include <sprite_reference.h>
#include <ufo_engine.h>

class Level;
class PingusLevel;
class WidgetSpriteReference;
class Button;

/// @spawn;
class WorldMap : public Widget{
public:
    Button* back_to_main_menu = nullptr;

    int currently_selected_location = 0;

    bool selected_to_left = false;

    bool level_was_selected = false;

    std::string last_played_level = "?";
    int last_played_level_rank = 3;

    WidgetSpriteReference* backdrop = nullptr;
    float backdrop_velocity = 0.0f;
    float backdrop_acceleration = 5000.0f;

    PingusLevel* level = nullptr;
    WorldMap(Vector2f _) : Widget(Vector2f(0.0f,0.0f), Engine::Get().pixel_game_engine.GetWindowSizeInPixles()){}

    void OnLevelEnter(Level* _level);

    void OnStart(Level* _level);
    void OnUpdate();
    void OnWidgetDraw();

};