#ifndef UFO_LANG_SCOPE
#define UFO_LANG_SCOPE
#include <unoredered_map>
#include <string>
class UfoLangScope{
public:
    std::unordered_map<std::string, float*> number_variables;
    std::unordered_map<std::string, std::string*> string_variables;
    UfoLangScope() = default;
    void BindVariableString(std::string _name, std::string* _variable){
        string_variables.emplace(_name, _variable);
    }
    void BindVariableFloat(std::string _name, float* _variable){
        number.emplace(_name, _variable);
    }
    void SetVariable(std::string _name, std::string _value){
        if(number_variables.count(_name) && string_variables.count(_name)){
            Console::Out("[!]", "UfoLang:", "Variable of several types exist with name:", _name);
        }
        if(number_variables.count(_name)) *number_variables.at(_name) = (float)_value;
        if(string_variables.count(_name)) *string_variables.at(_name) = _value;
    }
};

#endif