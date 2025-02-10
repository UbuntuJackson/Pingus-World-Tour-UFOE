#ifndef TALKABLE_H
#define TALKABLE_H

#include <memory>
#include "../json/json.h"
#include "../animation/animated_sprite_reference.h"
#include "../animation/animation.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"
#include "../ufo_maths/ufo_maths.h"
#include "tuxtale_level.h"
#include "tuxtale_player.h"
#include "../widgets/one_dimensional_text_box.h"

using namespace ufoMaths;

class Talkable : public Shape<Circle>{
public:
    std::string text;
    OneDimensionalTextBox* tb = nullptr;
    TuxTaleLevel* level;

    Talkable(Vector2f _local_position) : Shape<Circle>(Circle(olc::vf2d(16.0f, 16.0f), 6.0f), _local_position, false, olc::vf2d(8.0f, 8.0f)){

    }

    void OnLevelEnter(Level* _level){
        auto u_save_point = std::make_unique<SpriteReference>(
            "info_block",
            Vector2f(0.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(16.0f, 16.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        );

        level = dynamic_cast<TuxTaleLevel*>(_level);
        level->talkable_handles.push_back(this);

        AddChild(std::move(u_save_point));
    }

    void OnUpdate(){
        Shape<Circle>::OnUpdate();
        if(tb){
            if(SingleKeyboard::Get().GetKey(olc::N).is_pressed){
                bool has_next = tb->Next();
                if(!has_next) tb = nullptr;
            }
            if(SingleKeyboard::Get().GetKey(olc::B).is_pressed){
                bool has_previous = tb->Back();
            }
        }
    }

    void OnHit(TuxTalePlayer* _player){

        auto u_text_box = std::make_unique<OneDimensionalTextBox>(Vector2f(20.0f,240.0f-20.0f-50.0f), Vector2f(340.0-40.0f, 53.0f)); 

        u_text_box->text_queue.push_back(text);

        std::reverse(u_text_box->text_queue.begin(), u_text_box->text_queue.end());

        tb = u_text_box.get();

        level->NewActor(std::move(u_text_box));
    }

    static std::unique_ptr<Talkable> Load(Json& _json){
        std::string text_;
        Json::ArrayForEach(Json(), _json.GetObject("properties"),[&](Json& _object, Json _arr){
            if(_object.GetAsString("name") == "text"){
                text_ = _object.GetAsString("value");
            }
        });

        auto u_talkable = std::make_unique<Talkable>(Vector2f((float)_json.GetAsInt("x"), (float)_json.GetAsInt("y")-16.0f));
        u_talkable->text = text_;
        return std::move(u_talkable);
    }

};

#endif