#pragma once
#include <string>

class ExampleClass{
public:
    int id = 1;

    /// @export ;
    std::string name = "";

    ///@exp ort;
    float angle = 0.0f;

    ExampleClass(int _id, std::string _name){
        id = _id;
        name = _name;
    }
};