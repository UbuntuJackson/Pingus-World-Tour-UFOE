#include <vector>
#include "range.h"

namespace ufo{
/*std::vector<int> Range(int _start, int _end, int _step){
    std::vector<int> v;
    for(int i = _start; i < _end; i+=_step){
        v.push_back(i);
    }
    return v;
}*/
std::vector<int> Range(int _start, int _end, int _step){
    std::vector<int> v;
    int i = _start;
    while((_end-i)*(_end-_start) > 0){
        v.push_back(i);
        i+=_step;
    }
    return v;
}

std::vector<int> Range(int _start){
    int i = 0;
    std::vector<int> v;
    while(i < _start){
        v.push_back(i);
        i++;
    }
    return v;
}
}