#ifndef ACTOR_H
#define ACTOR_H

#include <memory>
#include <vector>
#include "../json/json_variant.h"
#include "../external/olcPixelGameEngine.h"
#include "../shapes/rectangle.h"

//To avoid circular dependancies between level.h and actor.h.
class Camera;
class Level;

//What is an Actor? There are three definitions:
//1.) An Actor is an object that inhibits in a level.
//2.) An Actor ACTS as a game-object.
//3.) An Actor is anything that inherits from baseclass Actor.

//Conventions:
//Always add Actors with Level::NewActor.
//Do NOT create actors as a non-unique_ptr.
//Do NOT call delete or new on Actor.

//Lifetime:
//Actors die with the Level unless explicitly moved in OnTransition(Level*).

class Actor{
public:
    //Experimental, not used by default.
    int reference_count = 0;

//private:
    int id = -1;
    //id in the level-file, assigned by the editor.
    int editor_id = -1;
    int z_index = 0;
    olc::vf2d position = {0.0f, 0.0f};
//public:
    olc::vf2d local_position = {0.0f, 0.0f};

    //Components.
    std::vector<std::unique_ptr<Actor>> children;
    std::vector<std::unique_ptr<Actor>> new_children_queue;
//private:
    //dead flag is either set directly or implicitly
    //Explicitly set if QueueForPurge() called on it
    //Implicitly set if child of dead object.
    //Check the purge-step of the level update cycle.
    bool dead = false;

    //Unused
    bool has_handle = false;

    //If not directly in level, or not child of Actor that is in level,
    //then this Actor is not in the level-tree.
    bool in_level_tree = false;
//public:
    //Unused so far
    ufo::Rectangle editor_selection_shape = ufo::Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(16.0f,16.0f));
    std::unique_ptr<Actor> GetConfigurationWidget();

    //Accessible as soon as OnLevelEnter(Level*) is called.
    Actor* parent = nullptr;

//public:
    //Every Actor has a unique ID. An ID n cannot be replicated unless the Engine is restarted.
    int GetID();

    //This is to be used when getting the global position of an Actor (name position)
    olc::vf2d GetGlobalPosition();
    
    //Add a component, in the UFO-Engine it's referred to as 'child'.
    void AddChild(std::unique_ptr<Actor> _actor);

    //Alias for AddChild, but also returns a handle to the child that was added.
    template <typename tActor, typename ...tArgs>
    tActor* AddChild(tArgs&& ..._args){
        auto u_actor = std::make_unique<tActor>(_args...);
        auto actor_ptr = u_actor.get();
        AddChild(std::move(u_actor));
        return actor_ptr;
    }

    //Utilised by the component-system. Handles each queued actor in new_children_queue.
    void AddQueuedChildren();

    //Always call this when inheriting from Actor.
    Actor(olc::vf2d _local_position);

    //Added this just to experiment with GarbageCollected actor
    Actor();
    virtual ~Actor();
    Actor(Actor&&) = default;
    
    //This runs recursively when NewActor is called, or when AddChild is called
    //and if this Actor is attached to an actor which is in level-tree.
    //All OTHER actors in the level might not be loaded when this is called.
    virtual void OnLevelEnter(Level* _level);
    virtual void LevelEnter(Level* _level);

    //OnStart is like a cousin to OnlevelEnter. It's called when everything is already loaded from
    //the Editor.
    virtual void OnStart(Level* _level);

    //Is called when the level transitions into another level.
    virtual void OnTransition(Level* _level);

    //To be deprecated for GetGlobalPosition()
    virtual void UpdateGlobalPosition(olc::vf2d _position);
    virtual void Update(olc::vf2d _position);
    
    //Runs every frame. Does not pass in deltatime, but you can
    //get it by calling Engine::Get().GetDeltaTime();
    virtual void OnUpdate();

    //Almost like OnUpdate, but you can call it whenever you want.
    virtual void HandleUpdate();

    //Actor has a built-in paused state. This will run in place of OnUpdate if
    //Level::paused == true.
    virtual void OnPaused();

    //Used to clean up dead actors and components. This searches the level-tree recursively.
    virtual void SearchForDeadActors(int _id);
    virtual void ReportAsDead(int _id);
    virtual void PurgeDeadActors();

    //New Z-Index. Change the Z-Index, tells the engine to resort std::vector actors which it
    //Otherwise does nto do.
    void SetZIndex(int _z_index);

    //Do not overwrite this unless you know what you are doing.
    //This in turn calls OnDraw(Camera*).
    virtual void Draw(Camera* _camera);

    //Draw-event. Write whatever you want in it, but keep in mind that parent classes might
    //Utilise it.
    virtual void OnDraw(Camera* _camera);

    //Runs only if all-shapes-visible is set.
    virtual void OnDebugDraw(Camera* _camera);

    //This in turn calls OnDebugDraw.
    virtual void DebugDraw(Camera* _camera);

    //Same as with Draw and OnDraw, but draws as it's own HUD-step, on top of everything else.
    virtual void WidgetDraw();
    virtual void OnWidgetDraw();

    virtual void PrintAttributes();

    virtual void OnSave(JsonVariant* _current_save_file);

    //QueueForPurge() will queue the actor to be purged within the current frame,
    //and should not exist in the next frame.
    //Handles to this Actor are not purged automatically, and aren't nulled automatically.
    //The responsibility of cleaning up handles is on the programmer.
    //Should be renamed Free()?
    virtual void QueueForPurge();
    //Runs before Actor is destructed.
    virtual void OnPurge(Level* _level);

    //Returns the type as std::string.
    virtual std::string GetType();
};

#endif