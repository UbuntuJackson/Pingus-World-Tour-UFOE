#ifndef COLLECTABLE_UPDATER_H
#define COLLECTABLE_UPDATER_H

template<typename tMap>
void CheckOverlapCollectable(tMap _collectables, tMap _grabbers){
    for(auto&& collectable : _collectables){
        for(auto&& grabber : _grabbers){
            if(/*overlapping*/) OnGrabbed(grabber ,collectable);
        }
    }
}

void OnGrabbed(Coin _coin, PlayerBody _pb){
    _pb.profile+=_coin->value;
}

void OnGrabbed(Coin* _coin, StonksPlayerBody* _pb){
    _pb->profile.coins+=_coin->value;
    int coins_to_invest_in_stonks = _pb->profile.coins * 0.5f;
    stonks+=coins_to_invest_in_stonks;
}

#endif