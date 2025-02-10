#ifndef OVERLAP_TEST_LEVEL
#define OVERLAP_TEST_LEVEL

#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../level/level.h"
#include "../mouse/mouse.h"
#include "../shapes/shape.h"
#include "../shapes/rectangle.h"
#include "../shapes/circle.h"
#include "../shapes/pill.h"
#include "../random/random_number_generator.h"
#include "../ufo_maths/ufo_maths.h"
#include "../console/console.h"

class OverlapTestLevel : public Level{
    Shape<Circle>* circle_ptr = nullptr;
    Shape<Pill>* pill_ptr = nullptr;
    std::vector<Shape<Circle>*> circle_ptrs;
    
    void OnResourceLoad(){
        //auto u_circle = std::make_unique<Shape<Circle>>(Circle(olc::vf2d(0.0f, 0.0f), 10.0f), olc::vf2d(0.0f, 0.0f));
        //circle_ptr = u_circle.get();
        
        auto u_pill = std::make_unique<Shape<Pill>>(Pill(olc::vf2d(0.0f, 0.0f), 10.0f, 20.0f), olc::vf2d(0.0f, 0.0f));
        pill_ptr = u_pill.get();

        NewActor(std::move(u_pill));
    }
    
    void OnUpdate(){
        GetActiveCamera()->clamp = false;
        Level::OnUpdate();

        olc::vf2d world_position = GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition());
    
        pill_ptr->local_position = world_position;

        if(Mouse::Get().GetLeftButton().is_released){
            float rand_radius = RandomNumberGenerator::Get().RandomFloat(0.0f, 100.0f);

            auto u_circle = std::make_unique<Shape<Circle>>(Circle(olc::vf2d(0.0f, 0.0f), rand_radius), world_position);
            circle_ptrs.push_back(u_circle.get());
            NewActor(std::move(u_circle));
        }

        pill_ptr->shape.colour = olc::VERY_DARK_BLUE;
        for(const auto& rect : circle_ptrs){
            if(ufoMaths::PillVsCircle(pill_ptr->shape, rect->shape)){
                pill_ptr->shape.colour = olc::GREEN;
                Console::Out("Ovelap");
            }
        }
    }
};

#endif