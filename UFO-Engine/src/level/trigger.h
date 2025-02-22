#ifndef TRIGGER
#define TRIGGER

#include "../actor/actor.h"
#include "level.h"
#include "../shapes/shape.h"
#include "../shapes/rectangle.h"

class Trigger : public Shape<ufo::Rectangle>{
public:
    Trigger(ufo::Rectangle _rectangle) : Shape<ufo::Rectangle>(_rectangle, _rectangle.position){}
    virtual void OnHit(Level* _level){
        
    }
};

#endif