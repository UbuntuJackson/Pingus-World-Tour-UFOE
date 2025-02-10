#ifndef UFO_TIME_H
#define UFO_TIME_H

class ufoTime{
public:
    ufoTime() = default;
    float delta_time;
    static ufoTime& Get(){
        static ufoTime time;
        return time;
    }
    static float GetDeltaTime();
};

#endif