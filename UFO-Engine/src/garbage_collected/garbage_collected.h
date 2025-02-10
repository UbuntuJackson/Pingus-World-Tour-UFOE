#ifndef GARBAGE_COLLECTED
#define GARBAGE_COLLECTED

#include <memory>
#include "../level/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "../camera/camera.h"
#include "../level/level.h"

template<typename tType>
class GarbageCollected : public Actor{
public:
    tType* object = nullptr;

    GarbageCollected() : Actor(olc::vf2d(0.0f, 0.0f)){
        Console::Out("default-constructed, create ID:", id);
    }

    GarbageCollected(tType* _object) : Actor(olc::vf2d(0.0f, 0.0f)){
        //if(_object != nullptr){
            Console::Out("create ID:", id);
            object = _object;
            _object->reference_count++;
            Console::Out(object->reference_count, "ref count up");
        //}
    }

    GarbageCollected(const GarbageCollected& _copy){
        object = _copy.object;
        object->reference_count++;
        Console::Out(object->reference_count, "User define copy constructor?");
    }

    GarbageCollected& operator=(const GarbageCollected& _copy){
        object = _copy.object;
        object->reference_count++;
        if(&_copy == this){
            Console::Out("copy is this");
            //return *this;
        }
        
        return *this;
    }

    GarbageCollected Reference(){
        Console::Out("User define copy constructor?");
        return GarbageCollected(object);
    }

    ~GarbageCollected(){
        Console::Out("destruct ID:", id);
        object->reference_count--;
        Console::Out(object->reference_count, "ref count down");
        if(object->reference_count == 0){
            Console::Out("[GarbageCollected<T>::~GarbageCollected()] An object was garbage collected.");
            delete object;
        }
    }

    virtual void OnLevelEnter(Level* _level){
        object->OnLevelEnter(_level);
    }
    void LevelEnter(Level* _level){
        Actor::LevelEnter(_level);
        object->LevelEnter(_level);
    }

    void UpdateGlobalPosition(olc::vf2d _position){
        object->UpdateGlobalPosition(_position);
    }
    void Update(olc::vf2d _position){
        Actor::Update(_position);
        object->Update(_position);
        position = object->position;
    }
    
    void OnUpdate(){
        //Console::Out(object->position);
        object->OnUpdate();
    }
    virtual void HandleUpdate(){
        object->HandleUpdate();
    }

    void SearchForDeadActors(int _id){
        Actor::SearchForDeadActors(_id);
        object->SearchForDeadActors(_id);
    }
    void ReportAsDead(int _id){
        Actor::ReportAsDead(_id);
        object->ReportAsDead(_id);
    }
    void PurgeDeadActors(){
        Actor::PurgeDeadActors();
        object->PurgeDeadActors();
    }

    void Draw(Camera* _camera){
        object->Draw(_camera);
    }
    virtual void OnDraw(Camera* _camera){
        object->OnDraw(_camera);
    }
    void WidgetDraw(){
        object->WidgetDraw();
    }
    virtual void OnWidgetDraw(){
        object->OnWidgetDraw();
    }

    void PrintAttributes(){
        object->PrintAttributes();
    }

    void OnSave(Json* _current_save_file){
        object->OnSave(_current_save_file);
    }

    //void QueueForPurge(){
        //object->QueueForPurge();
    //}
    virtual void OnPurge(Level* _level){
        object->OnPurge(_level);
    }

    tType* Get(){ //Only when calling methods, please.
        return object;
    }
};

template<typename tType, typename ...tArgs>
std::unique_ptr<GarbageCollected<tType>> MakeGarbageCollected(tArgs&& ..._args){
    return std::make_unique<GarbageCollected<tType>>(new tType(_args...));
}

#endif