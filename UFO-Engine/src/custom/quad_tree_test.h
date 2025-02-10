#ifndef QUAD_TREE_TEST
#define QUAD_TREE_TEST

#include <vector>
#include <memory>
#include "../level/level.h"
#include "../external/olcPixelGameEngine.h"
#include "../shapes/raw_shape_base.h"
#include "../shapes/shape.h"
#include "../shapes/rectangle.h"
#include "../camera/camera.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../console/console.h"

class QuadTreeTest : public Level{
public:

    std::vector<std::unique_ptr<Rectangle>> my_rectangles;
    Rectangle other_rectangle = Rectangle(olc::vf2d(410.0f, 410.0f), olc::vf2d(240.0f, 200.0f));

    Rectangle selection = Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(0.0f, 0.0f));

    class StaticQuadTree{
    public:
        int depth = -1;
        int MAX_DEPTH = 3;

        olc::vf2d position;
        olc::vf2d size;
        std::vector<Rectangle*> rectangle_handles;

        std::vector<StaticQuadTree> subdivisions;

        StaticQuadTree() = default;

        StaticQuadTree(int _depth, olc::vf2d _position, olc::vf2d _size) : position{_position}, size{_size}{
            //Sets depth and increases it for the next number of children
            depth = _depth;
            depth++;

            Console::Out("Quad at depth:", depth);

            if(depth < MAX_DEPTH){
                subdivisions.push_back(StaticQuadTree(depth, _position,size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, _position+olc::vf2d(0.0f,size.y/2.0f),size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, _position+olc::vf2d(size.x/2.0f, 0.0f),size/2.0f));
                subdivisions.push_back(StaticQuadTree(depth, _position+size/2.0f,size/2.0f));
            }
        }

        void Insert(Rectangle* _rectangle){
            Console::Out("Insertion at depth:", depth);

            rectangle_handles.push_back(_rectangle);

            for(auto&& subdivision : subdivisions){
                if(ufoMaths::RectangleVsRectangle(Rectangle(subdivision.position, subdivision.size), *_rectangle)){
                    subdivision.Insert(_rectangle);
                }
            }
        }

        void Resize(olc::vf2d _size){
            size = _size;
        }

        void Clear(){

        }



        std::vector<Rectangle*> Search(Rectangle* _area){
            std::vector<Rectangle*> rectangles;

            Search(_area, rectangles);

            return rectangles;
        }

        void Search(Rectangle* _area, std::vector<Rectangle*>& _rectangles){
            Console::Out("Depth for search:", depth);

            bool is_in_subdivision = false;
            for(auto&& subdivision : subdivisions){
                
                if(ufoMaths::RectangleContainsRectangle(Rectangle(subdivision.position, subdivision.size), *_area)){
                    
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

    void OnResourceLoad(){
        static_quad_tree = StaticQuadTree(0, olc::vf2d(0.0f, 0.0f), olc::vf2d(800.0f, 800.0f));
        my_rectangles.push_back(std::make_unique<Rectangle>(olc::vf2d(100.0f, 20.0f), olc::vf2d(200.0f, 100.0f)));
        my_rectangles.push_back(std::make_unique<Rectangle>(olc::vf2d(210.0f, 220.0f), olc::vf2d(50.0f, 90.0f)));
        my_rectangles.push_back(std::make_unique<Rectangle>(olc::vf2d(290.0f, 260.0f), olc::vf2d(10.0f, 10.0f)));
        
        //Adding rects to quadtree
        for(auto&& r : my_rectangles) static_quad_tree.Insert(r.get());
    }
    void OnUpdate(){
        Level::OnUpdate();

        //Updating selection rectangle.
        if(Mouse::GetLeftButton().is_pressed){
            selection.position = GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
        }
        if(Mouse::GetLeftButton().is_held){
            selection.size = GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition()) - selection.position;
        }

        //Clamping the camera every frame, this could be done in a better way
        GetActiveCamera()->clamp = false;

        //Just testing the RectangleContainsRectangle function
        if(ufoMaths::RectangleContainsRectangle(selection, other_rectangle)){
            auto transformed_rect = GetActiveCamera()->Transform(other_rectangle);
            Engine::Get().pixel_game_engine.DrawRectDecal(transformed_rect.position, transformed_rect.size, olc::GREEN);
        }
        else{
            auto transformed_rect = GetActiveCamera()->Transform(other_rectangle);
            Engine::Get().pixel_game_engine.DrawRectDecal(transformed_rect.position, transformed_rect.size, olc::RED);
        }

        static_quad_tree.Draw(GetActiveCamera());

        std::vector<Rectangle*> v = static_quad_tree.Search(&selection);
        Console::Out("Found",v.size());

        for(const auto& rect : my_rectangles){
            auto trec = GetActiveCamera()->Transform(*(rect.get()));
            Engine::Get().pixel_game_engine.DrawRectDecal(trec.position, trec.size, olc::BLUE);
        }

        auto selecrec = GetActiveCamera()->Transform(selection);
        Engine::Get().pixel_game_engine.DrawRectDecal(selecrec.position, selecrec.size, olc::GREEN);


        
    }
};

#endif