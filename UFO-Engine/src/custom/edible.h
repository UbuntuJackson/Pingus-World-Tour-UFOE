#ifndef EDIBLE_H
#define EDIBLE_H

#include "frogatto.h"
#include "../level/actor.h"
#include "../shapes/raw_shape_base.h"
#include "frogatto_level.h"

class Level;
class Edible : public Actor{
public:
    Edible();

    virtual void OnEaten(Frogatto* _frogatto);
    virtual bool CheckOverlap(RawShapeBase* _shape);
    
};

template<typename tActor>
class TEdible : public Edible{
public:
    tActor* actor;
    RawShapeBase* shape_handle;
    TEdible(tActor* _actor, RawShapeBase* _shape_handle) : Edible(), actor{_actor}, shape_handle{_shape_handle}{}

    void OnEaten(Frogatto* _frogatto){
        actor->OnEaten(_frogatto);
        Console::Out("virtual OnEaten");
    }

    void OnLevelEnter(Level* _level){
        FrogattoLevel* level = dynamic_cast<FrogattoLevel*>(_level);
        level->edible_handles.push_back(this);
    }

    bool CheckOverlap(RawShapeBase* _shape){
        return _shape->IsOverlappingOther(shape_handle);
    }

    void OnPurge(Level* _level){
        actor->QueueForPurge();
    }
};

#endif