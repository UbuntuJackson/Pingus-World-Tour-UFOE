#include "../console/console.h"
#include <map>
#include <vector>
#include <string>

std::map<std::string, std::vector<std::string>> ufoMenuLangExecuteTest(std::string_view _code){
    int character_index = 0;
    std::string read = "";
    std::string command = "";
    std::vector<std::string> args;
    std::map<std::string, std::vector<std::string>> command_args_map;
    while(character_index != _code.size()){
        char character = _code[character_index];
        character_index++;
        if(character == '('){
            command = read;
            read = "";
            continue;
        }
        if(character == ',' || character == ')'){
            args.push_back(read);
            read = "";
            continue;
        }
        if(character == ';'){
            command_args_map[command] = {};
            for(auto&& arg : args) command_args_map.at(command).push_back(arg);
            args.clear();
            continue;
        }
        read += character;
    }
    return command_args_map;
}

int main(){
    
    for(auto&& [k, v] : ufoMenuLangExecuteTest("LoadScene(Hello.json);")){
        Console::Out("function-name:", k);
        for(auto&& arg : v) Console::Out("arguments:" ,arg);
    }

    for(auto&& [k, v] : ufoMenuLangExecuteTest("Save();")){
        Console::Out("function-name:", k);
        for(auto&& arg : v) Console::Out("arguments:" ,arg);
    }

    for(auto&& [k, v] : ufoMenuLangExecuteTest("Slider(0,100); Colour(0,100,255);")){
        Console::Out("function-name:", k);
        for(auto&& arg : v) Console::Out("arguments:" ,arg);
    }
    return 0;
}