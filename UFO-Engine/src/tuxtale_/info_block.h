#pragma once
#include "floating_block.h"
#include "../ufo_maths/ufo_maths.h"

class Level;
class TuxTalePlayer;
class Level;
class SpriteReference;
class OneDimensionalTextBox;

class InfoBlock : public FloatingBlock{
public:
    SpriteReference* sprite_ref;
    bool empty = false;
    std::string text;
    OneDimensionalTextBox* tb = nullptr;

    InfoBlock(Vector2f _local_position);

    void OnLevelEnter(Level* _level);

    void OnUpdate();

    virtual void OnHit(TuxTalePlayer* _level);
    void OnOutOfRange(TuxTalePlayer* _player);

    static std::unique_ptr<InfoBlock> Load(Json* _json){
        std::string text_;
        Json::ArrayForEach(Json(), _json->GetObject("properties"),[&](Json& _object, Json _arr){
            if(_object.GetAsString("name") == "text"){
                text_ = _object.GetAsString("value");
            }
        });

        auto u_talkable = std::make_unique<InfoBlock>(Vector2f((float)_json->GetAsInt("x"), (float)_json->GetAsInt("y")-16.0f));
        u_talkable->text = text_;
        return std::move(u_talkable);
    }

};