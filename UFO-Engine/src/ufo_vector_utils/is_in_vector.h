#ifndef IS_IN_VECTOR_H
#define IS_IN_VECTOR_H

#include <algorithm>
#include <vector>

template<typename tType>
bool IsInVector(std::vector<tType> &_v, tType _element){
    if(std::find(_v.begin(), _v.end(), _element) == _v.end()){
        return false;
    }
    return true;
}

#endif