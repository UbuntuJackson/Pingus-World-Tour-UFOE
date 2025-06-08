#pragma once
#include <ufo_maths.h>
#include <rectangle.h>
#include <ufo_engine.h>
#include <graphics.h>

class StaticQuadTree;

class QuadTreeRectangle{
public:
    Vector2f position;
    Vector2f size;
    Vector2f velocity = {0.0f, 0.0f};
    Colour colour = Graphics::WHITE;
    StaticQuadTree* quad_tree = nullptr;

    //You could traverse up the quadtree to filter out some unnecessary searches
    bool to_be_removed = false;

    QuadTreeRectangle(Vector2f _position, Vector2f _size);

    ufo::Rectangle GetRectangle();

    void OnUpdate();

    void Remove();

    void OnDraw(Camera* _camera);
};