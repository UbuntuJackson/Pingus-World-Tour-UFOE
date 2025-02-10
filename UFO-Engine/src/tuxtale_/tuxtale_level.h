#ifndef TUXTALE_LEVEL
#define TUXTALE_LEVEL

#include "../external/olcPixelGameEngine.h"
#include "../level/level.h"
#include "../ufo_maths/ufo_maths.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"

class Enemy;
class TuxTaleCoin;
class Talkable;
class Actor;
class TuxTaleCollectable;
class FileMenu;
class FloatingBlock;
class ControllableUFO;

struct TileCollisionData{
    bool place_free;
    std::vector<int> tiles;
};

class TuxTaleLevel : public Level{
public:
    float gravity_scale = 1.0f;

    std::vector<Actor*> save_point_handles;
    FileMenu* debug_menu = nullptr;

    std::vector<Enemy*> enemy_handles;
    std::vector<TuxTaleCollectable*> collectable_handles;
    std::vector<Talkable*> talkable_handles;
    std::vector<FloatingBlock*> floating_block_handles;
    std::vector<ControllableUFO*> vehicle_handles;

    void OnResourceLoad();
    void OnLoad(Json& _level_json);

    TileCollisionData PlaceFree(const Rectangle& _rectangle);
    TileCollisionData PlaceFree(const Circle& _rectangle);
    TileCollisionData PlaceFreeControllableUFO(const Circle& _rectangle);

    void OnLoadActors(Json& _actor_json);

    void OnLoadLayer(Json& _layer_json);

    void OnExit();

    void OnUpdate();

    void OnHandlePurge();

    void OnSave(Json* _current_save);

    void OnSaveFileLoad(Json* _current_save);
};

#endif