#pragma once
#include <ufo_maths.h>
#include <rectangle.h>
#include <ufo_engine.h>
#include <graphics.h>

class QuadTreeRectangle{
public:
    Vector2f position;
    Vector2f size;
    Vector2f velocity = {0.0f, 0.0f};
    Colour colour = Graphics::WHITE;
    QuadTreeRectangle(Vector2f _position, Vector2f _size) : position{_position}, size{_size}{}

    ufo::Rectangle GetRectangle(){
        return ufo::Rectangle(position, size);
    }

    void OnUpdate(){
        position += velocity * Engine::Get().GetDeltaTime();
    }

    void OnDraw(Camera* _camera){
        Graphics::Get().DrawRectangleFilled(_camera->Transform(position), _camera->scale * size, colour);
        colour = Graphics::WHITE;
    }
};