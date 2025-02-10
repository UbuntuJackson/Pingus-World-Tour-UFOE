#ifndef FIND_IN_VECTOR_H
#define FIND_IN_VECTOR_H

#include <algorithm>
#include <vector>

template<typename tType>
bool FindInVector(std::vector<tType> &_v, tType _element){
    if(std::find(_v.begin(), _v.end(), _element) == _v.end()){
        return false;
    }
    return true;
}

#endif