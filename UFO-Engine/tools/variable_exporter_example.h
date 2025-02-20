#pragma once
#include <string>

class ExampleClass{
public:
    int number = 2;

    ///     @export;
    std::string   text;

    ///@export;
    bool is_true = false;

    ///   @export
    int my_var = 19;

///@export
    float my_float;

    ///@exportt
    double pun_int_ended;

    /// @export
    double le;

    ExampleClass(int _number, std::string _text){
        number = _number;
        text = _text;
    }
};