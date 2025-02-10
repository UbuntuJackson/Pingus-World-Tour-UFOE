#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H
#include <unordered_map>
#include <map>
#include <string>

#include "animation_bundle.h"
#include "../external/olcPixelGameEngine.h"

class AnimationManager{
public:
    static AnimationManager& Get(){
        static AnimationManager self;
        return self;
    }

    void Load(std::string _folder, std::string _file_name, std::string _name, std::string _initial_state);
    void Remove(std::string _name);
    AnimationBundle* GetAnimationBundle(std::string _name);

    std::unordered_map<std::string, AnimationBundle> animation_bundles;
};

#endif