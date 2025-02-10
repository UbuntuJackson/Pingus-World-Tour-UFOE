#ifndef TIMER
#define TIMER
#include <chrono>
#include "../console/console.h"
class Timer{
public:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    float number_of_seconds_to_count = 0.0f;
    bool is_started = false;
    Timer(){
        start_time = std::chrono::steady_clock::now();
        is_started = false;
    };
    void Start(float _number_of_seconds_to_count){
        number_of_seconds_to_count = _number_of_seconds_to_count;
        start_time = std::chrono::steady_clock::now();
        is_started = true;
    }
    void Stop(){
        is_started = false;
    }
    float GetTimeLeft(){
        return number_of_seconds_to_count - std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::steady_clock::now() - start_time)).count();
    }
};

#endif