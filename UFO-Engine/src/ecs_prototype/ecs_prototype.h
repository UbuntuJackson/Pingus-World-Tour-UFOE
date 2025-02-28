#include <map>
#include <vector>
#include <algorithm>

class Component{

};

enum Types{
    INT,
    STRING,
    FLOAT
};

struct Component{
    int type;
    void* data;
};

class ECS{
public:
    typedef int TypeId;

    std::map<int, Component> entities;

    std::vector<std::vector<Component*>> Find(std::vector<int> _types){
        std::vector<std::vector<Component*>> found_entities;

        for(auto [k, entity] : entities){
            std::vector<int>::iterator it;
            
            it = std::find(_types.begin(), _types.end(), entity.type);

            if(it != _types.end()){
                
            }
        }
    }

    void Update(){

        for(auto&& entity : entities){
            std::map<TypeId, void*>::iterator i = actor.find(Types::INT);

            std::map<TypeId, void*>::iterator f = actor.find(Types::FLOAT);
            
            if(i != actor.end() && f != actor.end()){
                
            }
        }
    }

};