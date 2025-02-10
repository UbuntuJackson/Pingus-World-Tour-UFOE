#include <map>
#include <vector>

class Component{

};

enum Types{
    INT,
    STRING,
    FLOAT
};

class ECS{
public:
    typedef int TypeId;

    std::vector<std::map<TypeId, void*>> actors;

    void Update(){

        for(auto&& actor : actors){
            std::map<TypeId, void*>::iterator i = actor.find(Types::INT);

            std::map<TypeId, void*>::iterator f = actor.find(Types::FLOAT);
            
            if(i != actor.end() && f != actor.end()){
                
            }
        }
    }

};