#pragma once
#include <vector>
#include <actor.h>
#include <ufo_maths.h>
#include <rectangle.h>
#include <mouse.h>
#include <olcPixelGameEngine.h>
#include <ufo_engine.h>
#include <level.h>
#include <random_number_generator.h>

class QuadTree : public Actor{
public:
    /// @export;
    int max_depth = 2;

    QuadTree(Vector2f _local_position) : Actor(Vector2f(0.0f,0.0f)){
        
    }

    std::vector<std::unique_ptr<ufo::Rectangle>> my_rectangles;
    ufo::Rectangle other_rectangle = ufo::Rectangle(olc::vf2d(410.0f, 410.0f), olc::vf2d(240.0f, 200.0f));

    ufo::Rectangle selection = ufo::Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(0.0f, 0.0f));

    class StaticQuadTree{
    public:
        int depth = -1;

        int MAX_DEPTH = 6;

        olc::vf2d position;
        olc::vf2d size;
        std::vector<ufo::Rectangle*> rectangle_handles;

        std::vector<StaticQuadTree> subdivisions;

        StaticQuadTree() = default;

        StaticQuadTree(int _depth, olc::vf2d _position, olc::vf2d _size) : position{_position}, size{_size}{
            //Sets depth and increases it for the next number of children
            depth = _depth;
            depth++;

            Console::Out("Quad at depth:", depth);

            /*if(depth < MAX_DEPTH){
                subdivisions.push_back(StaticQuadTree(depth, _position,size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, _position+olc::vf2d(0.0f,size.y/2.0f),size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, _position+olc::vf2d(size.x/2.0f, 0.0f),size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, _position+size/2.0f,size/2.0f));
            }*/
        }

        void Insert(ufo::Rectangle* _rectangle){
            Console::Out("Insertion at depth:", depth);

            rectangle_handles.push_back(_rectangle);
            Console::Out("MAX_DEPTH is:", MAX_DEPTH);

            if(rectangle_handles.size() == 4 && depth < MAX_DEPTH){
                subdivisions.push_back(StaticQuadTree(depth, position,size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, position+olc::vf2d(0.0f,size.y/2.0f),size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, position+olc::vf2d(size.x/2.0f, 0.0f),size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, position+size/2.0f,size/2.0f));
                for(auto&& subdivision : subdivisions){
                    for(auto&& rectangle_handle : rectangle_handles){
                        if(ufoMaths::RectangleVsRectangle(ufo::Rectangle(subdivision.position, subdivision.size), *rectangle_handle)){
                            subdivision.Insert(rectangle_handle);
                        }
                    }
                }
            }

            for(auto&& subdivision : subdivisions){
                if(ufoMaths::RectangleVsRectangle(ufo::Rectangle(subdivision.position, subdivision.size), *_rectangle)){
                    subdivision.Insert(_rectangle);
                }
            }
        }

        void Resize(olc::vf2d _size){
            size = _size;
        }

        void Clear(){

        }



        std::vector<ufo::Rectangle*> Search(ufo::Rectangle* _area){
            std::vector<ufo::Rectangle*> rectangles;

            Search(_area, rectangles);

            return rectangles;
        }

        void Search(ufo::Rectangle* _area, std::vector<ufo::Rectangle*>& _rectangles){
            Console::Out("Depth for search:", depth);

            bool is_in_subdivision = false;
            for(auto&& subdivision : subdivisions){
                
                if(ufoMaths::RectangleContainsRectangle(ufo::Rectangle(subdivision.position, subdivision.size), *_area)){
                    
                    is_in_subdivision = true;
                    subdivision.Search(_area, _rectangles);
                    
                }
            }

            if(depth == MAX_DEPTH || !is_in_subdivision){
                for(auto&& i : rectangle_handles){
                    _rectangles.push_back(i);
                    Console::Out("rectangle at depth", depth);
                }
                return;
            }
        }

        void Draw(Camera* _camera){
            Engine::Get().pixel_game_engine.DrawRectDecal(_camera->Transform(position), size*_camera->scale, olc::MAGENTA);
            for(auto& subdivision : subdivisions){
                subdivision.Draw(_camera);
            }
        }
    };

    StaticQuadTree static_quad_tree;

    void OnStart(Level* _level){
        
        SetZIndex(7);

        static_quad_tree = StaticQuadTree(0, olc::vf2d(0.0f, 0.0f), olc::vf2d(800.0f, 800.0f));
        static_quad_tree.MAX_DEPTH = max_depth;
        //Console::Out("MAX_DEPTH",static_quad_tree.MAX_DEPTH);

        my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(100.0f, 20.0f), olc::vf2d(200.0f, 100.0f)));
        my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(210.0f, 220.0f), olc::vf2d(50.0f, 90.0f)));
        my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(290.0f, 260.0f), olc::vf2d(10.0f, 10.0f)));
        
        for(int i = 0; i < 100; i++){
            int xx = RandomNumberGenerator::Get().RandomInt(0,static_quad_tree.size.x);
            int yy = RandomNumberGenerator::Get().RandomInt(0,static_quad_tree.size.y);
            int ww = RandomNumberGenerator::Get().RandomInt(0,200);
            int hh = RandomNumberGenerator::Get().RandomInt(0,200);
            my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(xx,yy), olc::vf2d(ww, hh)));
        }

        //Adding rects to quadtree
        for(auto&& r : my_rectangles) static_quad_tree.Insert(r.get());

    }
    void OnDraw(Camera* _camera){

        //Updating selection rectangle.
        if(Mouse::Get().GetLeftButton().is_pressed){
            selection.position = _camera->TransformScreenToWorld(Mouse::Get().GetPosition());
        }
        if(Mouse::Get().GetLeftButton().is_held){
            selection.size = _camera->TransformScreenToWorld(Mouse::Get().GetPosition()) - selection.position;
        }

        //Clamping the camera every frame, this could be done in a better way
        //GetActiveCamera()->clamp = false;

        //Just testing the RectangleContainsRectangle function
        /*if(ufoMaths::RectangleContainsRectangle(selection, other_rectangle)){
            auto transformed_rect = _camera->Transform(other_rectangle);
            Engine::Get().pixel_game_engine.DrawRectDecal(transformed_rect.position, transformed_rect.size, olc::GREEN);
        }
        else{
            auto transformed_rect = _camera->Transform(other_rectangle);
            Engine::Get().pixel_game_engine.DrawRectDecal(transformed_rect.position, transformed_rect.size, olc::RED);
        }*/

        static_quad_tree.Draw(_camera);

        std::vector<ufo::Rectangle*> v = static_quad_tree.Search(&selection);
        Console::Out("Found",v.size());

        Console::Out("Drawing QuadTree");

        for(const auto& rect : my_rectangles){
            auto trec = _camera->Transform(*(rect.get()));
            Engine::Get().pixel_game_engine.DrawRectDecal(trec.position, trec.size, olc::BLUE);
        }

        for(const auto& rect : v){
            auto trec = _camera->Transform(*(rect));
            Engine::Get().pixel_game_engine.DrawRectDecal(trec.position, trec.size, olc::RED);
        }

        auto selecrec = _camera->Transform(selection);
        Engine::Get().pixel_game_engine.DrawRectDecal(selecrec.position, selecrec.size, olc::GREEN);


        
    }
};