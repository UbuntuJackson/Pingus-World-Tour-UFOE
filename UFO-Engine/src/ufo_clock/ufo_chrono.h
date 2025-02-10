#include <chrono>

namespace ufo{
    typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;
}

class ufoChrono{
public:

    static TimePoint Now(){
        return std::chrono::steady_clock::now();
    }
    /*template<typename tUnit>
    static AsDuration(ufo::TimePoint _tp){
        return 
    }*/
};