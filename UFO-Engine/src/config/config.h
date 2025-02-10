#include "../json/ufo_json.h"
#include <string>
#include <map>

class Config{
public:
    std::map<std::string, std::string> contents;
    void Read(std::string _path){
        Json j = Json::Read(_path);
        for(auto&& element : j.GetAsArray("config")){
            contents[element.GetAsString("name")] = element.GetAsString("value");
        }
    }
    void Save(){
        Json j = Json::New();
        for(auto&& [k, v] : contents){
            Json obj = Json::New();
            obj.PushToObject(k, v);
            j.PushToArray(obj);
        }
    }
    static Config New(std::map<std::string, std::string> _m){
        auto c = Config();
        c.contents = _m;
        return c;
    }
    static Config New(){
        auto c = Config();
        return c;
    }
    void Set(std::string _s, std::string _other_s){
        contents.at(_s) = _other_s;
    }
    void NewEntry(std::string _s, std::string _other_s){
        contents[_s] = _other_s;
    }
    void EraseEntry(std::string _s){
        contents.erase(_s);
    }
};