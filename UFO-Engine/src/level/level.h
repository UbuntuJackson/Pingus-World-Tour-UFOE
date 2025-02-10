#ifndef LEVEL_H
#define LEVEL_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "../tilemap/tilemap.h"
#include "../ufo_vector_utils/find_in_vector.h"
#include "tcollision_system.h"
#include "../collision/collision_system.h"
#include "../json/json.h"
#include "../shapes/rectangle.h"
#include "../bitmap_level/bitmap_dynamic_solid.h"
#include "../bitmap_level/bitmap_collision_body.h"

class RawShapeBase;
class SpriteReference;
class Widget;
class Camera;
class PlayerBody;
class BackgroundImage;
class Trigger;
class LevelTrigger;
class SpawnPoint;

enum SpawnMode{
    SPAWN_POINT,
    SAVEFILE,
    GAME_START
};

class Level{
public:
    Level() = default;
    std::string path;
    int level_format = 0;
    std::string level_class = "Level";
    bool retain_player_properties = false;

    SpawnMode spawn_mode = SpawnMode::GAME_START;
    olc::vf2d spawn_point;
    std::string spawn_point_name = "";

    bool paused = false;
    bool should_resort_after_z_index = false;
    std::function<bool(const std::unique_ptr<Actor>& _a, const std::unique_ptr<Actor>& _b)> actor_sorting_method = [&](const std::unique_ptr<Actor>& _a, const std::unique_ptr<Actor>& _b){
        if(_a->z_index == _b->z_index){
            return _a->GetGlobalPosition().y < _b->GetGlobalPosition().y;
        }
        
        return _a->z_index < _b->z_index;
    };

    olc::vf2d level_size; //level size in units, not tiles.
    Tilemap tilemap;
    bool tilemap_exists = true;
    TCollisionSystem tcollision_system;
    CollisionSystem collision_system;

    std::unordered_map<std::string, olc::Decal*> level_decals;
    std::vector<BitmapDynamicSolid*> dynamic_bitmap_solid_handles;
    std::vector<BitmapCollisionBody*> bitmap_collision_body_handles;

    //olc::Sprite empty_sprite;

    olc::Sprite pixel_effect_sprite;
    std::unique_ptr<olc::Decal> pixel_effect_decal;

    std::vector<std::unique_ptr<Actor>> actors; //could assume every actor to have a position.
    std::vector<std::unique_ptr<Actor>> new_actor_queue;
    std::vector<SpriteReference*> sprite_handles;
    std::vector<Actor*> collision_body_handles;
    std::vector<Camera*> camera_handles;
    std::vector<PlayerBody*> player_body_handles;
    std::vector<BackgroundImage*> background_image_handles;
    std::vector<Trigger*> trigger_handles;
    std::vector<LevelTrigger*> level_trigger_handles;
    std::vector<LevelTrigger*> hit_level_triggers; //Should level triggers automatically set spawn_mode = SpawnMode::SPAWN_POINT?
    std::vector<SpawnPoint*> spawn_point_handles;
    std::vector<Actor*> savable_actor_handles;
    std::vector<Actor*> transitionable_actor_handles;

    std::vector<Widget*> widget_handles;

    std::vector<int> queued_for_purge;
    std::vector<int> additional_queued_for_purge;

    void NewActor(std::unique_ptr<Actor> _actor);

    template <typename tActor, typename ...tArgs>
    tActor* NewActor(tArgs&& ..._args){
        auto u_actor = std::make_unique<tActor>(_args...);
        auto actor_ptr = u_actor.get();
        NewActor(std::move(u_actor));
        return actor_ptr;
    }

    template<typename tActor>
    std::vector<tActor*> GetActorsOfType(){
        std::vector<tActor*> actors_from_type;
        for(const auto& actor : actors){
            tActor* a = dynamic_cast<tActor*>(actor.get());
            if(a != nullptr) actors_from_type.push_back(a);
        }
        return actors_from_type;
    }

    template<typename tType>
    void PurgeHandles(std::vector<tType*>& _handles){
        if(queued_for_purge.size() != 0){
            Console::Out("queued for purge:");
            Console::Out(queued_for_purge);
        }

        for(int i = _handles.size()-1; i >= 0; i--){
            if(FindInVector(queued_for_purge, _handles[i]->id)){
                Console::Out("should delete handle:",_handles[i]->id);
                _handles.erase(_handles.begin()+i);
            }
        }
    }

    virtual void OnLoadJsonCustom(Json& _json);
    void Load();
    virtual void OnLoad(Json& _level_json);
    virtual void OnLoadLayer(Json& _layer_json);
    void LoadActors(Json& _json);
    virtual void OnLoadActors(Json& _actor_json);
    //template<typename tActor> LoadActor(){
    //
    //}
    virtual void OnResourceLoad();

    void InitializeEditMode();

    Camera* GetActiveCamera();
    void ActiveCameraLookAround(bool _condition_move, bool _condition_zoom);

    void Update();
    virtual void OnUpdate();
    virtual void OnPaused();
    void Draw();
    virtual void OnBackgroundElementDraw(Camera* _camera);
    void DebugDraw();
    void QueueForPurge(int _id);
    void PurgeQueuedForKill();
    virtual void OnHandlePurge();

    virtual void OnTransition(Level* _level);
    virtual void OnSaveFileLoad(Json* _json);
    virtual void OnSave(Json* _current_savefile);
    virtual void Save(Json* _current_savefile);
    virtual void OnExit();

    virtual ~Level();

};

#endif