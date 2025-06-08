#include <ufo_maths.h>
#include <rectangle.h>
#include <ufo_engine.h>
#include <graphics.h>
#include "quadtree_rectangle.h"

QuadTreeRectangle::QuadTreeRectangle(Vector2f _position, Vector2f _size) : position{_position}, size{_size}{}

ufo::Rectangle QuadTreeRectangle::GetRectangle(){
    return ufo::Rectangle(position, size);
}

void QuadTreeRectangle::OnUpdate(){
    position += velocity * Engine::Get().GetDeltaTime();
}

void QuadTreeRectangle::Remove(){
    /*

    for(int i = int(quad_tree->rectangle_handles.size())-1; i >= 0; i--){

        if(quad_tree->rectangle_handles[i] == this){
            rectangle_handles.erase(rectangle_handles.begin()+i);
        }

        quad_tree->parent->Remove(this);

    }
    
    */
}

void QuadTreeRectangle::OnDraw(Camera* _camera){
    Graphics::Get().DrawRectangleFilled(_camera->Transform(position), _camera->scale * size, colour);
    colour = Graphics::WHITE;
}