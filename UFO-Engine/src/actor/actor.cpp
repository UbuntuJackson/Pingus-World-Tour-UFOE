#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "actor.h"
#include "../ufo_engine/ufo_engine.h"
#include "../console/console.h"
#include "../keyboard/single_keyboard.h"
#include "../json/json.h"
#include "../level/level.h"

int Actor::GetID(){
    return id;
}

olc::vf2d Actor::GetGlobalPosition(){
    if(parent == nullptr){
        return local_position;
    }
    return local_position + parent->GetGlobalPosition();
}

void Actor::AddChild(std::unique_ptr<Actor> _actor){
    _actor->parent = this;
    if(in_level_tree){
        _actor->LevelEnter(Engine::Get().current_level.get());
    }
    new_children_queue.push_back(std::move(_actor));
    Console::Out("Added child");
}

void Actor::AddQueuedChildren(){
    
    for(auto&& i : new_children_queue){
        children.push_back(std::move(i));
    }

    for(const auto& child : children){
        child->AddQueuedChildren();
    }

    UpdateGlobalPosition(olc::vf2d(0.0f, 0.0f));

    for(const auto& child : children){
        child->OnStart(Engine::Get().current_level.get());
    }

    new_children_queue.clear();
}

Actor::Actor(olc::vf2d _local_position) :
local_position{_local_position}
{
    id = Engine::Get().GenerateID();
}

Actor::Actor(){
    id = Engine::Get().GenerateID();
}

Actor::~Actor(){
    //Console::Out("Actor destruct", id);
}

void Actor::OnLevelEnter(Level* _level){

}
void Actor::LevelEnter(Level* _level){
    in_level_tree = true;
    OnLevelEnter(_level);
    _level->should_resort_after_z_index = true;

    for(const auto& child : children){
        child->LevelEnter(_level);
    }
}

void Actor::OnStart(Level* _level){

}

void Actor::OnTransition(Level* _level){
    
}

void Actor::UpdateGlobalPosition(olc::vf2d _position){
    position = _position+local_position;
    for(const auto& child : children){
        child->UpdateGlobalPosition(position);
    }
    
}

void Actor::Update(olc::vf2d _position){
    
    position = _position+local_position;

    for(const auto& child : children){
        child->Update(position);
    }
    
    OnUpdate();
    position = _position+local_position; //experimental
    

    if(SingleKeyboard::Get().GetKey(olc::A).is_pressed) PrintAttributes();
}

void Actor::OnUpdate(){

}

void Actor::HandleUpdate(){}

void Actor::OnPaused(){}

void Actor::SearchForDeadActors(int _id){
    if(id == _id){
        ReportAsDead(_id); //this should only happen if a handle exists.
    }
    else{
        for(const auto& child : children){
            child->SearchForDeadActors(_id);
        }
    }
}
void Actor::ReportAsDead(int _id){
    dead = true;
    if(_id != id) Engine::Get().current_level->additional_queued_for_purge.push_back(id);
    for(const auto& child : children){
        child->ReportAsDead(_id);
    }
}

void Actor::PurgeDeadActors(){
    for(int i = children.size()-1; i >= 0; i--){
        if(children[i]->dead){
            children.erase(children.begin()+i);
        }
        else{
            children[i]->PurgeDeadActors();
        }
    }
}

void Actor::SetZIndex(int _z_index){
    z_index = _z_index;
    Engine::Get().current_level->should_resort_after_z_index = true;
}

void Actor::Draw(Camera* _camera){
    OnDraw(_camera);
    for(const auto& child : children){
        child->Draw(_camera);
    }
}

void Actor::WidgetDraw(){
    OnWidgetDraw();
    for(const auto& child : children){
        child->WidgetDraw();
    }
}

void Actor::OnWidgetDraw(){}

void Actor::PrintAttributes(){
    Console::Out("Position:", position);
    Console::Out("Local position:", local_position);
}

void Actor::OnDraw(Camera* _camera){}

void Actor::OnDebugDraw(Camera* _camera){

}

void Actor::DebugDraw(Camera* _camera){
    for(const auto& child : children){
        child->DebugDraw(_camera);
    }
    OnDebugDraw(_camera);
}

void Actor::OnSave(Json* _current_save_file){}

void Actor::QueueForPurge(){
    auto local_level = Engine::Get().current_level.get();
    
    local_level->QueueForPurge(id);
    OnPurge(local_level);
}

void Actor::OnPurge(Level* _level){}

std::string Actor::GetType(){
    return "Actor";
}