#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H
#include <vector>
#include <memory>
#include "../scene/scene.h"
#include <string>

class SceneSystem{
public:
    SceneSystem();
    void Start();
    bool quit;
    void Quit();

    typedef std::function<bool()> Event; //Event is a function that the engine will delete if the event returns true
    typedef std::function<void(SceneSystem* _scene_system)> SceneTransitionEvent;
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<Event> events;
    std::vector<SceneTransitionEvent> scene_transition_events;
    std::vector<Event> start_events;
    //std::vector<std::unique_ptr<Scene>> scenes_to_be_added;
    //std::vector<int> scenes_to_be_removed_from_index;

    template<typename tScene, typename ...tArgs>
    tScene* NewScene(tArgs ... _args){
        std::unique_ptr<tScene> scene = std::make_unique<tScene>(_args...);
        tScene* scene_raw = scene.get();
        //scene->scene_system = this;
        scenes.push_back(std::move(scene));
        return scene_raw;
    }

    template<typename tScene, typename ...tArgs>
    tScene* LoadScene(std::string _path, tArgs ... _args){
        std::unique_ptr<tScene> scene = std::make_unique<tScene>(_args...);
        tScene* scene_raw = scene.get();
        scene->scene_system = this;
        scene_raw->Load(_path);
        scene_raw->OnStart();
        scenes.push_back(std::move(scene));
        return scene_raw;
    }

    template<typename tScene, typename ...tArgs>
    void ReplaceScene(tArgs ..._args){
        scenes.pop_back();
        scenes.push_back(_args...);
    }
    
    void GotoScene(std::string _scene_name);
    void RemoveActiveScene();

    Scene* GetActiveScene();

    void Update();
};

#endif