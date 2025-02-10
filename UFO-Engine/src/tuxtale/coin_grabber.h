#ifndef COIN_GRABBER_H
#define COIN_GRABBER_H

#include "../shapes/rectangle.h"
#include "../shapes/dynamic_shape.h"

class CoinGrabber{
public:
    int profile_index;
    DynamicShape<Rectangle>* shape;
    static int RESPONSE = GenerateResponseID();
    int count;
    void Update(int _value, Rectangle* _rectangle){
        if(ufoMaths::RectangleVsRectangle(shape, _rectangle)){
            coins+=_value;
            _rectangle.add_response(CoinGrabber::RESPONSE);
        }
    }
    void OnSave(Json* _save){
        Json profile = _save.GetObject(std::to_string(profile_index));
        Json coins = Json::NewObject(count);
        profile.PushToObject("coins", coins);
    }
};