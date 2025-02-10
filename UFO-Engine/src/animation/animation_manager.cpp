#include <string>
#include <utility>

#include "animation_manager.h"
#include "../console/console.h"
#include "animation_bundle.h"
#include <unordered_map>

void AnimationManager::Load(std::string _folder, std::string _file_name, std::string _name, std::string _initial_state){
    if(animation_bundles.count(_name)){
        Console::Out("Animation Bundle with name:", "'", _name, "'", "already exists.");
        return;
    }
    animation_bundles.emplace(std::piecewise_construct,
        std::forward_as_tuple(_name), std::forward_as_tuple(_folder, _file_name, _initial_state));
}
void AnimationManager::Remove(std::string _name){
    if(!animation_bundles.count(_name)) animation_bundles.erase(_name);
    else Console::Out("[!]", "Key for animation not found");
}

AnimationBundle* AnimationManager::GetAnimationBundle(std::string _name){
    return &animation_bundles.at(_name);
}