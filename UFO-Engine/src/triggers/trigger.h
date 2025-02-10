#ifndef TRIGGER_H
#define TRIGGER_H
#include <string>
#include <unordered_map>

template<typename tShape>
struct Trigger{
public:
    tShape* shape;
    std::string code;
};

struct ufoLangScope{
    std::unoredered_map<std::string, float*>
    std::unoredered_map<std::string, std::string*>
    void ExposeVariable(std::string _name, float* _ptr);
    void ExposeVariable(std::string _name, std::string* _ptr);
    void Set(std::string _name, float _value);
}

class TriggerSensitive{
public:
    TriggerSensitive() = default;
    tShape* shape;
    virtual void OnTriggerFound(Scene* _scene, std::string _code){
        
    }
};

template<typename tType>
class MyTriggerSensitive : public TriggerSensitive{
    TriggerSensitive(tType* _parent){
        
    }
    tType* parent;
    void OnTriggerFound(Scene* _scene, std::string _code){

    }
}

#endif