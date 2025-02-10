#ifndef BALLOON_POP
#define BALLOON_POP
#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../level/ufo_engine.h"
#include "../camera/camera.h"
#include "../console/console.h"

class BalloonPop : public Actor{
public:
    std::vector<olc::vf2d> dots;
    olc::Pixel colour; 
    BalloonPop(olc::vf2d _position, olc::Pixel _colour, int _number_of_dots = 16) : Actor(_position), colour{_colour}{
        for(int i = 0; i < _number_of_dots; i++) dots.push_back(olc::vf2d(0.0f, 0.0f));
    }
    void OnUpdate(){
        auto local_level = Engine::Get().current_level.get();
        for(int angle = 0; angle < dots.size(); angle++){

            float r_angle = (float)angle/(float)dots.size()*ufoMaths::PI*2.0f;

            //Just testing out the home-made cos and sin function
            dots[angle]+=olc::vf2d(ufoMaths::Cos(r_angle),ufoMaths::Sin(r_angle))*7.7f;
            
            local_level->pixel_effect_decal->sprite->SetPixel(local_level->GetActiveCamera()->Transform(position+dots[angle]), colour);
        }
    }
};

#endif