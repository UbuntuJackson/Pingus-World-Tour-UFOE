#ifndef CIRCLE_VS_LINE_LEVEL
#define CIRCLE_VS_LINE_LEVEL

#include <vector>
#include <memory>
#include <cmath>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/circle.h"
#include "../shapes/ray2.h"
#include "../shapes/shape.h"
#include "../level/level.h"
#include "../level/ufo_engine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../mouse/mouse.h"

class CircleVsLineLevel : public Level{
public:
    std::vector<Shape<Circle>*> my_circles;
    Ray2 my_ray = Ray2(olc::vf2d(9.0f, 9.0f),olc::vf2d(79.0f, 89.0f));

    void OnResourceLoad(){

    }

    void OnUpdate(){
        GetActiveCamera()->clamp = false;
        Level::OnUpdate();

        if(Mouse::GetLeftButton().is_pressed){
            auto u_circle = std::make_unique<Shape<Circle>>(
                Circle(olc::vf2d(0.0f, 0.0f), 50.0f),
                GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition()), true
            );
            my_circles.push_back(u_circle.get());

            NewActor(
                std::move(u_circle)
            );
        }

        
        my_ray.p1 = GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());

        bool hit = false;

        for(auto&& circle : my_circles){
            RayVsCircleCollisionData result = ufoMaths::RayVsCircle(my_ray,circle);
            
            Console::Out("Collision normal:",result.collision_normal);
            

            if(result.is_hit) hit = true;

            if(result.is_hit) Engine::Get().pixel_game_engine.DrawLineDecal(GetActiveCamera()->Transform(result.intersection_point), GetActiveCamera()->Transform(result.intersection_point+result.collision_normal*20.0f), olc::GREEN);
        }

        //float v1 = ufoMaths::PositiveReducedQuadraticEquation(-1.0f,0.5f,8.0f);
        //float v2 = ufoMaths::NegativeReducedQuadraticEquation(-1.0f,0.5f,8.0f);

        //Console::Out(v1,v2);

        if(hit){
            Engine::Get().pixel_game_engine.DrawLineDecal(GetActiveCamera()->Transform(my_ray.Start()), GetActiveCamera()->Transform(my_ray.End()), olc::GREEN);
        }
        else{
            Engine::Get().pixel_game_engine.DrawLineDecal(GetActiveCamera()->Transform(my_ray.Start()), GetActiveCamera()->Transform(my_ray.End()), olc::DARK_GREY);
        }

    }
};

#endif