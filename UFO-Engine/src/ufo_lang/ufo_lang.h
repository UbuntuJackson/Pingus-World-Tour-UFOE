#ifndef UFO_LANG_H
#define UFO_LANG_H
#include <string>
#include <vector>

class Scene;
class ufoLang{
public:
    struct Call{
        std::string command;
        std::vector<std::string> args;
    };
    ufoLang() = default;
    int depth = 0;
    static ufoLang& Get(){
        static ufoLang self;
        return self;
    }
    //std::vector<std::string> execution_stack;
    static void Execute(Scene* _scene, std::string _code);
    static std::string ExecuteAndReturn(Scene* _scene, std::string _code);
    static std::string DoCommand(Scene* _scene, const std::string& _command, const std::vector<std::string>& _args);
    static void Test(Scene* _scene);
};
#endif