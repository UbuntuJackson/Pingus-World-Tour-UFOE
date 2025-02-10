#ifndef COLOUR_SHAPE_H
#define COLOUR_SHAPE_H

#include "../drawing_system/draw.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"

class ColourShapeBase{
public:
    bool permanently_visible = false;
    ColourShapeBase(bool _permanently_visible) : permanently_visible{_permanently_visible}{}
    virtual void Update(Camera* _camera) = 0;
};

template<typename tShape>
class ColourShape : public ColourShapeBase{
public:
    olc::Pixel colour;
    tShape* shape;
    ColourShape(tShape* _shape, olc::Pixel _colour, bool _permanently_visible) :
        shape{_shape},
        colour{_colour},
        ColourShapeBase(_permanently_visible)
        {}
    void Update(Camera* _camera){
        DrawingSystem::Draw(*shape, _camera, colour);
    }
};

/*
class ColourRay : public ColourShapeBase{
public:
    olc::Pixel colour;
    Ray2* ray;
    ColourShape(Ray2* _ray, olc::Pixel _colour, bool _permanently_visible) :
        ray{_ray},
        colour{_colour},
        ColourShape(_permanently_visible)
        {}
    void Update(Camera* _camera){

    }
};
*/

#endif