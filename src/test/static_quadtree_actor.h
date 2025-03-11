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
#include <ufo_benchmarker.h>
#include "quadtree_rectangle.h"

class StaticQuadTreeActor : public Actor{
public:
    /// @export;
    int max_depth = 2;

    /// @export;
    int number_of_rectangles = 10;

    /// @export;
    int max_width = 200;

    /// @export;
    int max_height = 200;

    int number_of_subdivision_containment_checks = 0;

    bool with_quad_tree = true;

    StaticQuadTreeActor(Vector2f _local_position) : Actor(Vector2f(0.0f,0.0f)){
        
    }

    std::vector<std::unique_ptr<QuadTreeRectangle>> my_rectangles;
    ufo::Rectangle other_rectangle = ufo::Rectangle(olc::vf2d(410.0f, 410.0f), olc::vf2d(240.0f, 200.0f));

    ufo::Rectangle selection = ufo::Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(0.0f, 0.0f));

    class StaticQuadTree{
    public:
        int depth = -1;

        StaticQuadTreeActor* actor = nullptr;

        ufo::Rectangle rectangle = ufo::Rectangle(olc::vf2d(0.0f,0.0f), olc::vf2d(0.0f,0.0f));
        std::vector<QuadTreeRectangle*> rectangle_handles;

        std::vector<StaticQuadTree> subdivisions;

        StaticQuadTree() = default;

        StaticQuadTree(StaticQuadTreeActor* _actor ,int _depth, olc::vf2d _position, olc::vf2d _size) : actor{_actor}, rectangle(_position, _size){
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

        void Insert(QuadTreeRectangle* _rectangle){
            Console::Out("Insertion at depth:", depth);

            rectangle_handles.push_back(_rectangle);

            if(rectangle_handles.size() == 4 && depth < actor->max_depth){
                
                subdivisions.push_back(StaticQuadTree(actor, depth, rectangle.position,rectangle.size/2.0f));
                subdivisions.push_back(StaticQuadTree(actor, depth, rectangle.position+olc::vf2d(0.0f,rectangle.size.y/2.0f),rectangle.size/2.0f));
                subdivisions.push_back(StaticQuadTree(actor, depth, rectangle.position+olc::vf2d(rectangle.size.x/2.0f, 0.0f),rectangle.size/2.0f));
                subdivisions.push_back(StaticQuadTree(actor, depth, rectangle.position+rectangle.size/2.0f,rectangle.size/2.0f));

                //For all the rectangles that weren't added yet
                for(auto&& subdivision : subdivisions){
                    for(auto&& rectangle_handle : rectangle_handles){
                        if(ufoMaths::RectangleVsRectangle(subdivision.rectangle, rectangle_handle->GetRectangle())){
                            subdivision.Insert(rectangle_handle);
                        }
                    }
                }
                return;
            }

            //For the newly inserted rectangle
            for(auto&& subdivision : subdivisions){
                if(ufoMaths::RectangleVsRectangle(subdivision.rectangle, _rectangle->GetRectangle())){
                    subdivision.Insert(_rectangle);
                }
            }
        }

        void FindAndRemoveObjects(){
            
            for(int i = int(rectangle_handles.size())-1; i >= 0; i--){

                if(rectangle_handles[i]->to_be_removed){
                    rectangle_handles.erase(rectangle_handles.begin()+i);
                }

            }

            for(auto&& subdivision : subdivisions){
                subdivision.FindAndRemoveObjects();
            }
            
        }

        void Resize(olc::vf2d _size){
            rectangle.size = _size;
        }

        void Clear(){

        }



        std::vector<QuadTreeRectangle*> Search(ufo::Rectangle* _area){
            std::vector<QuadTreeRectangle*> rectangles;

            actor->number_of_subdivision_containment_checks = 0;

            Search(_area, rectangles);

            return rectangles;
        }

        void SearchOverlappingSubdivisions(ufo::Rectangle* _area, std::vector<QuadTreeRectangle*>& _rectangles){
            
            if(ufoMaths::RectangleVsRectangle(rectangle, *_area)){
                
                for(auto&& rectangle_handle : rectangle_handles){
                    _rectangles.push_back(rectangle_handle);
                }

            }
        }

        void Search(ufo::Rectangle* _area, std::vector<QuadTreeRectangle*>& _rectangles){
            //Console::Out("Depth for search:", depth);

            //bool should_search_overlapping_divisions = true;

            for(StaticQuadTree& subdivision : subdivisions){
                actor->number_of_subdivision_containment_checks++;
                if(ufoMaths::RectangleContainsRectangle(subdivision.rectangle, *_area)){
                    //Console::Out("Will return");
                    //Engine::Get().Quit();
                    //return;

                    subdivision.Search(_area,_rectangles);

                    return;
                }
                
            }

            if(subdivisions.size() == 0){
                //Console::Out("Reached bottom of quadtree at depth:",depth);
                //Console::Out("Number of subdivision checks", actor->number_of_subdivision_containment_checks);
                for(auto&& rectangle_handle : rectangle_handles){
                    _rectangles.push_back(rectangle_handle);
                }
                return;
            }

            for(auto&& subdivision : subdivisions){
                subdivision.SearchOverlappingSubdivisions(_area, _rectangles);
            }
        }

        void Draw(Camera* _camera){
            Engine::Get().pixel_game_engine.DrawRectDecal(_camera->Transform(rectangle.position), rectangle.size*_camera->scale, olc::MAGENTA);
            for(auto& subdivision : subdivisions){
                subdivision.Draw(_camera);
            }
        }
    };

    StaticQuadTree quad_tree;

    void OnStart(Level* _level){
        
        SetZIndex(7);

        quad_tree = StaticQuadTree(this, 0, olc::vf2d(0.0f, 0.0f), _level->level_size);
        //Console::Out("MAX_DEPTH",static_quad_tree.MAX_DEPTH);

        //my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(100.0f, 20.0f), olc::vf2d(200.0f, 100.0f)));
        //my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(210.0f, 220.0f), olc::vf2d(50.0f, 90.0f)));
        //my_rectangles.push_back(std::make_unique<ufo::Rectangle>(olc::vf2d(290.0f, 260.0f), olc::vf2d(10.0f, 10.0f)));
        
        for(int i = 0; i < number_of_rectangles; i++){
            int xx = RandomNumberGenerator::Get().RandomInt(0,quad_tree.rectangle.size.x);
            int yy = RandomNumberGenerator::Get().RandomInt(0,quad_tree.rectangle.size.y);
            int ww = RandomNumberGenerator::Get().RandomInt(0,max_width);
            int hh = RandomNumberGenerator::Get().RandomInt(0,max_height);
            my_rectangles.push_back(std::make_unique<QuadTreeRectangle>(olc::vf2d(xx,yy), olc::vf2d(ww, hh)));
        }

        //Adding rects to quadtree
        for(auto&& r : my_rectangles) quad_tree.Insert(r.get());

    }
    void OnDraw(Camera* _camera){

        if(SingleKeyboard::Get().GetKey(olc::Q).is_pressed){
            with_quad_tree = !with_quad_tree;
        }

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

        quad_tree.Draw(_camera);

        ufoBenchMarker u;

        if(with_quad_tree){
            std::vector<QuadTreeRectangle*> v = quad_tree.Search(&selection);
            //Console::Out("Found",v.size());

            //Console::Out("Drawing QuadTree");

            for(const auto& rect : v){
                rect->colour = olc::CYAN;
                if(ufoMaths::RectangleVsRectangle(rect->GetRectangle(), selection)){
                    rect->colour = olc::RED;
                }

                if(SingleKeyboard::Get().GetKey(olc::DEL).is_pressed){
                    rect->to_be_removed = true;
                }
            }

            quad_tree.FindAndRemoveObjects();
            
        }
        else{
            for(const auto& rect : my_rectangles){
                if(ufoMaths::RectangleVsRectangle(rect->GetRectangle(), selection)){
                    rect->colour = olc::RED;
                }
            }
        }
        Console::Out("Benchmark",u.Stop());

        /*ufoBenchMarker benchmark_rect_vs_rect;
        ufoMaths::RectangleVsRectangle(ufo::Rectangle(Vector2f(60.0f,60.0f),Vector2f(60.0f,60.0f)),ufo::Rectangle(Vector2f(10.0f,10.0f),Vector2f(70.0f,50.0f)));
        Console::Out("benchmark_rect_vs_rect",benchmark_rect_vs_rect.Stop());

        ufoBenchMarker benchmark_rect_cn_rect;
        ufoMaths::RectangleContainsRectangle(ufo::Rectangle(Vector2f(60.0f,60.0f),Vector2f(60.0f,60.0f)),ufo::Rectangle(Vector2f(10.0f,10.0f),Vector2f(70.0f,50.0f)));
        Console::Out("benchmark_rect_cn_rect",benchmark_rect_cn_rect.Stop());*/

        for(const auto& rect : my_rectangles){
            rect->OnDraw(_camera);
            //auto trec = _camera->Transform(*(rect.get()));
            //Engine::Get().pixel_game_engine.DrawRectDecal(trec.position, trec.size, olc::BLUE);
        }

        auto selecrec = _camera->Transform(selection);
        Graphics::Get().DrawRectangle(selecrec.position, selecrec.size, olc::GREEN);

        Graphics::Get().DrawString(Vector2f(10.0f,10.0f), "Using Quadtree:"+std::to_string(int(with_quad_tree)), olc::WHITE, Vector2f(1.0f,1.0f));
        
    }
};