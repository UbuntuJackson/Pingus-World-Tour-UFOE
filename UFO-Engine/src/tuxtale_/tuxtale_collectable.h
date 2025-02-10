#ifndef TUXTALE_COLLECTABLE
#define TUXTALE_COLLECTABLE

#include <memory>
#include "../json/json.h"
#include "../animation/animated_sprite_reference.h"
#include "../animation/animation.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"
#include "../ufo_maths/ufo_maths.h"
#include "tuxtale_level.h"
#include "tuxtale_player.h"

class TuxTaleCollectable : public Shape<Circle>{
public:
    bool is_collectable = true;

    TuxTaleCollectable(Vector2f _position) : Shape<Circle>(Circle(Vector2f(16.0f, 16.0f), 6.0f), _position, false, Vector2f(8.0f, 8.0f)){
        
    }

    void OnLevelEnter(Level* _level){
        Shape<Circle>::OnLevelEnter(_level);
        if(is_collectable) dynamic_cast<TuxTaleLevel*>(_level)->collectable_handles.push_back(this);
        
    }

    void OnUpdate(){
        Shape<Circle>::OnUpdate();
    }

    virtual void OnCollected(TuxTalePlayer* _player){

    }
    
};

#endif