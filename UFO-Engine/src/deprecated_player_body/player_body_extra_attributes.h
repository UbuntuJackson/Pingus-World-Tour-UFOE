#include "player_body.h"

struct PlayerBodyExtraAttribute{

    virtual void Update(PlayerBody* _player_body);
};

struct DoubleJump : PlayerBodyExtraAttribute{
    bool did_double_jump = false;
    void Update(PlayerBody* _player_body){
        if(!did_double_jump){
            _player_body->Jump();
            did_double_jump = true;
        }
    }
};