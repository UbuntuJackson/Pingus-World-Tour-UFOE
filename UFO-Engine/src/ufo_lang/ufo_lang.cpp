#include <string>
#include <vector>
#include <cmath>
#include "ufo_lang.h"
#include "../scene/scene.h"
#include "../scene_system/scene_system.h"
#include "../console/console.h"
#include "../ufo_benchmarker/ufo_benchmarker.h"
#include "../file/file.h"

void
ufoLang::Execute(Scene* _scene, std::string _code){
    Console::Out("[!]", "ufo-lang trying to run:", _code);
    std::string read = "";
    std::string command = "";
    std::vector<std::string> args;
    std::vector<Call> menu_commands;

    int brackets_detected = 0;
    int end_brackets_detected = 0;
    bool executable_code_was_added_as_argument = false;
    bool reading_string = false;

    for(const char& character : _code){
        if(character == '(') brackets_detected++;
        if(character == ')') end_brackets_detected++;
        if(character == '"'){
            reading_string = !reading_string;
            if(brackets_detected > 1) read+=character;
            continue;
        }
        if(character == ' ' && reading_string){
            read+=character;
            continue;
        }
        if(character == ' ') continue;
        if(character == '\t') continue;
        if(character == '\n') continue;

        
        if(brackets_detected > 1){
            read+=character;

            if(end_brackets_detected == brackets_detected -1){
                brackets_detected = 1;
                end_brackets_detected = 0;
                //Console::Out(read, "ExecuteAndReturn");
                if(command == "if"){
                    args.push_back(read);
                }
                else{
                    args.push_back(ExecuteAndReturn(_scene, read));
                }
                read = "";
                executable_code_was_added_as_argument = true;  
            }
            continue;
        }

        if(character == '('){
            command = read;
            read = "";
            continue;
        }
        if(character == ',' || character == ')'){
            if(executable_code_was_added_as_argument){
                executable_code_was_added_as_argument = false;
                continue;
            }
            args.push_back(read);
            read = "";
            continue;
        }
        if(character == ';'){
            menu_commands.push_back(Call{command, args});
            brackets_detected = 0;
            end_brackets_detected = 0;
            args.clear();
            command = "";
            continue;
        }
        read += character;
    }
    for(const auto& menu_command : menu_commands){
        DoCommand(_scene, menu_command.command, menu_command.args);
    }
}

std::string
ufoLang::ExecuteAndReturn(Scene* _scene, std::string _code){
    int depth = ++ufoLang::Get().depth;
    std::string read = "";
    std::string command = "";
    std::vector<std::string> args;

    int brackets_detected = 0;
    int end_brackets_detected = 0;
    bool executable_code_was_added_as_argument = false;
    bool reading_string = false;
    //Console::Out("depth:",depth, "runs:", _code, "brackets:", brackets_detected);

    for(const char& character : _code){
        if(character == '(') brackets_detected++;
        if(character == ')') end_brackets_detected++;
        if(character == '"'){
            reading_string = !reading_string;
            if(brackets_detected > 1) read+=character;
            continue;
        }
        if(character == ' ' && reading_string){
            read+=character;
            continue;
        }
        if(character == ' ') continue;
        if(character == '\t') continue;
        if(character == '\n') continue;

        if(brackets_detected > 1){
            read+=character;

            if(end_brackets_detected == brackets_detected -1){
                brackets_detected = 1;
                end_brackets_detected = 0;
                //Console::Out(read, "ExecuteAndReturn");
                if(command == "if"){
                    args.push_back(read);
                }
                else{
                    args.push_back(ExecuteAndReturn(_scene, read));
                }
                executable_code_was_added_as_argument = true;
                read = "";  
            }
            continue;
        }

        if(character == '('){
            command = read;
            read = "";
            continue;
        }
        if(character == ','){
            if(executable_code_was_added_as_argument){
                executable_code_was_added_as_argument = false;
                continue;
            }
        }
        if(character == ')'){
            if(executable_code_was_added_as_argument){
                return DoCommand(_scene, command, args);
                continue;
            }
        }

        if(character == ',' || character == ')'){
            args.push_back(read);
            read = "";
            if(character == ')'){
                return DoCommand(_scene, command, args);
            }
            continue;
        }
        read += character;
    }
    return "";
}

std::string
ufoLang::DoCommand(Scene* _scene, const std::string& _command, const std::vector<std::string>& _args){
    if(_command == "LoadScene"){
        Console::Out("Attempting to call LoadScene");
        for(auto&& arg : _args){
            Console::Out(arg);
            _scene->scene_system->scene_transition_events.push_back([a = arg](SceneSystem* _s){_s->LoadScene<Scene>(a);});
        }
    }
    if(_command == "SetConfig"){
        //config.at(_args[0]) = _args[1];
    }
    if(_command == "LoadConfig"){
        //Config::LoadConfig(arg[0]); //gotta make a config class.
        Console::Out("Attempting to load Config");
    }
    if(_command == "SaveConfig"){
        //Config::SaveConfig(config);
        Console::Out("Attempting to save Config");
    }
    if(_command == "GotoEditor"){
        //LoadScene<DefaultEditor>(arg[0]);
    }
    if(_command == "Quit"){
        _scene->scene_system->Quit();
        Console::Out("Attempting to call Quit()");
    }
    if(_command == "ReturnNumber"){
        return _args[0];
    }
    if(_command == "add"){
        return std::to_string(std::stoi(_args[0]) + std::stoi(_args[1]));
    }
    if(_command == "sub"){
        return std::to_string(std::stoi(_args[0]) - std::stoi(_args[1]));
    }
    if(_command == "pow"){
        return std::to_string(std::pow(std::stoi(_args[0]),std::stoi(_args[1])));
    }

    if(_command == "out"){
        Console::Out("[!]", "ufo-lang output:", _args[0]);
    }

    if(_command == "=="){
        //Console::Out(_args[0], _args[1]);
        if(_args[0] == _args[1]) return "true";
        else return "false";
    }
    if(_command == "if"){
        if(ExecuteAndReturn(_scene,_args[0]) == "true"){
            Execute(_scene,_args[1]+";");
        }
        else if(_args.size() == 3){
            Execute(_scene,_args[2]+";");
        }
    }

    return "";
}

void ufoLang::Test(Scene* _scene){

    ufoBenchMarker b;

    std::string w = File().Read("../res/ufo_lang_scripts/showcase.ufol");
    Execute(_scene, w);
    
    Console::Out("[!]", "UFO-Lang test took:", b.Stop(), "nanoseconds");
}