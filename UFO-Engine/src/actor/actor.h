#ifndef ACTOR_H
#define ACTOR_H

#include <memory>
#include <vector>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/rectangle.h"

class Json;
class Camera;
class Level;

class Actor{
public:
    //Experimental, not used by default.
    int reference_count = 0;

//private:
    int id = -1;
    int editor_id = -1;
    int z_index = 0;
    olc::vf2d position = {0.0f, 0.0f};
//public:
    olc::vf2d local_position = {0.0f, 0.0f};
    std::vector<std::unique_ptr<Actor>> children;
    std::vector<std::unique_ptr<Actor>> new_children_queue;
//private:
    bool dead = false;
    bool has_handle = false;
    bool in_level_tree = false;
//public:
    //Unused so far
    ufo::Rectangle editor_selection_shape = ufo::Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(16.0f,16.0f));
    std::unique_ptr<Actor> GetConfigurationWidget();

    Actor* parent = nullptr;

//public:
    int GetID();

    olc::vf2d GetGlobalPosition();
    
    void AddChild(std::unique_ptr<Actor> _actor);

    template <typename tActor, typename ...tArgs>
    tActor* AddChild(tArgs&& ..._args){
        auto u_actor = std::make_unique<tActor>(_args...);
        auto actor_ptr = u_actor.get();
        AddChild(std::move(u_actor));
        return actor_ptr;
    }

    void AddQueuedChildren();

    Actor(olc::vf2d _local_position);

    //Added this just to experiment with GarbageCollected actor
    Actor();
    virtual ~Actor();
    Actor(Actor&&) = default;
    
    virtual void OnLevelEnter(Level* _level);
    virtual void LevelEnter(Level* _level);

    virtual void OnStart(Level* _level);

    virtual void OnTransition(Level* _level);

    //To be deprecated for GetGlobalPosition()
    virtual void UpdateGlobalPosition(olc::vf2d _position);
    virtual void Update(olc::vf2d _position);
    
    virtual void OnUpdate();
    virtual void HandleUpdate();

    virtual void OnPaused();

    virtual void SearchForDeadActors(int _id);
    virtual void ReportAsDead(int _id);
    virtual void PurgeDeadActors();

    void SetZIndex(int _z_index);
    virtual void Draw(Camera* _camera);
    virtual void OnDraw(Camera* _camera);
    virtual void OnDebugDraw(Camera* _camera);
    virtual void DebugDraw(Camera* _camera);
    virtual void WidgetDraw();
    virtual void OnWidgetDraw();

    virtual void PrintAttributes();

    virtual void OnSave(Json* _current_save_file);

    //Should be renamed Free()?
    virtual void QueueForPurge();
    virtual void OnPurge(Level* _level);

    virtual std::string GetType();
};

#endif