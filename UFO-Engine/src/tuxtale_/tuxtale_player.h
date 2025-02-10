#ifndef TUXTALE_PLAYER
#define TUXTALE_PLAYER

#include <memory>
#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../json/json.h"
#include "../animation/animation.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"

class OneDimensionalTextBox;
class Level;
class TuxTaleLevel;
class FileMenu;
class Camera;

using namespace ufoMaths;

class TuxTalePlayer : public Shape<Rectangle>{
public:
    int coin_count = 0;

    float walking_speed = 100.0f;

    bool took_step_last_frame = false;
    bool can_save = false;
    bool saved = false;
    bool bumped_head = false;
    bool hit_enemy = false;
    bool died = false;

    Animation* animation;
    SpriteReference* shadow = nullptr;

    Camera* camera = nullptr;
    float camera_offset_x = 0.0f;

    TuxTaleLevel* level;

    float jump_velocity = 0.0f;
    float initial_jump_velocity = -70.0f;
    float gravity = 220.0f;
    bool on_ground = false;

    OneDimensionalTextBox* tb = nullptr;

    TuxTalePlayer(Vector2f _position);

    void OnLevelEnter(Level* _level);
    void OnStart(Level* _level);

    void OnTransition(Level* _former_level);

    void OnUpdate();

    void OnPaused();

    static std::unique_ptr<TuxTalePlayer> Load(Json& _json);

    void OnLoad(Json* _current_save_file);

    void OnSave(Json* _current_save_file);

    //Deprecated
    bool PlaceFree(const Rectangle& _rectangle);

    void DieFromFall();

    void OnWidgetDraw();

};

#endif