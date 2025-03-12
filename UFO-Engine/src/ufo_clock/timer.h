#ifndef TIMER
#define TIMER
#include <chrono>
#include "../console/console.h"
class Timer{
public:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    float number_of_seconds_to_count = 0.0f;
    float time_left_when_paused = 0.0f;
    bool is_started = false;
    bool is_paused = false;
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
    void FastForward(float _multply_elapsed_time){
        if(!is_started) return;
        float reduced_time = GetTimeLeft() - Engine::Get().GetDeltaTime() * _multply_elapsed_time * 1000.0f;
        Start(reduced_time);
    }
    void Pause(){
        if(!is_paused) time_left_when_paused = GetTimeLeft();
        is_paused = true;
    }
    void Resume(){
        //Guard against starting the timer over continuously.
        if(is_paused) Start(time_left_when_paused);
        is_paused = false;
    }
};

#endif