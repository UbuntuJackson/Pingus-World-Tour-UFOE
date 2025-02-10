#pragma once

#include <string>
#include "../level/actor.h"
#include "../sprite_reference/sprite_reference.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../drawing_system/drawing_system.h"
using namespace ufoMaths;

class Comet : public Actor{
public:

    float speed = 0.0f;

    std::function<float(void)> func_calculation;

    std::string text;

    Comet(Vector2f _local_position,  std::function<float(void)> _func_calculation, std::string _text) : Actor(_local_position),
    func_calculation{_func_calculation},
    text{_text}
    {

    }

    void OnLevelEnter(Level* _level){
        AddChild(std::make_unique<SpriteReference>(
            "blue_comet",
            Vector2f(0.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f, 1.0f),
            0,
            0
        ));
    }

    void OnUpdate(){
        local_position.y += speed * Engine::Get().GetDeltaTime();
    }

    std::string GetText(){
        return text;
    }

    float GetAnswer(){
        return func_calculation();
    }

    bool ParseAnswer(std::string _ans){
        if(std::stoi(_ans) == GetAnswer()){
            return true;
        }

        return false;
    }

    void OnWidgetDraw(){
        //DrawingSystem::DrawString(text,local_position)
    }

};