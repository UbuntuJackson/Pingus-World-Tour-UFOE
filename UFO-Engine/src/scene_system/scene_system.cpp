#include "scene_system.h"
#include <string>
#include "../scene/scene.h"
#include <cassert>
#include "../console/console.h"

SceneSystem::SceneSystem(){
    quit = false;
}

void SceneSystem::Quit(){
    Console::Out("[!] [SceneSystem::Quit(): quit flag set to false. Execution ends at the end of game cycle.]");
    quit = true;
}

void SceneSystem::Start(){
    
    for(auto&& event : start_events) event();
    //if(scenes.size() > 0) scenes.back()->OnStart();
    
}

void
SceneSystem::GotoScene(std::string _scene_name){}

void
SceneSystem::RemoveActiveScene(){}

Scene*
SceneSystem::GetActiveScene(){
    assert(scenes.size() > 0);

    return scenes.back().get();
}

void
SceneSystem::Update(){
    scenes[scenes.size()-1]->Update();
    for(auto&& event : events){
        event();
    }
    events.clear();

    for(const auto& event : scene_transition_events){
        event(this);
    }
    scene_transition_events.clear();
    //scenes[scenes.size()-1]->DrawScene();
}