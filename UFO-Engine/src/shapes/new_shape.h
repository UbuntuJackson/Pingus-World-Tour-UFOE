#ifndef NEW_SHAPE
#define NEW_SHAPE
#include "../external/olcPixelGameEngine.h"
#include "../creation_data/creation_data.h"
#include "../scene/scene.h"
#include "colour_shape.h"
#include "ray2.h"

CreationData<Rectangle> NewRectangle(Scene* _scene, float _x, float _y, float _width, float _height, const olc::Pixel& _colour = olc::WHITE, bool _visible_by_default = false);
CreationData<Circle> NewCircle(Scene* _scene, float _x, float _y, float _radius, const olc::Pixel& _colour = olc::WHITE, bool _visible_by_default = false);
template<typename tShape>
CreationData<ColourShape<tShape>> NewColourShape(Scene* _scene, tShape* _shape, const olc::Pixel& _colour = olc::WHITE, bool _visible_by_default = false){
    auto creation_data = _scene->AddNewActorToMap<ColourShape<tShape>>(_scene->colour_shapes, _shape, _colour, _visible_by_default);
    return creation_data;
}
CreationData<Ray2> NewUserRay2(Scene* _scene, olc::vf2d _start, olc::vf2d _end, const olc::Pixel& _colour = olc::WHITE, bool _visible_by_default = false);
CreationData<Ray2> NewLineSegment2(Scene* _scene, olc::vf2d _start, olc::vf2d _end, const olc::Pixel& _colour = olc::WHITE, bool _visible_by_default = false);

#endif