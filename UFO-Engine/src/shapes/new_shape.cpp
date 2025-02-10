#include "new_shape.h"
#include "../external/olcPixelGameEngine.h"
#include "rectangle.h"
#include "circle.h"
#include "colour_shape.h"
#include "../scene/scene.h"
#include "../creation_data/creation_data.h"
#include "ray2.h"

CreationData<Rectangle> NewRectangle(Scene* _scene, float _x, float _y, float _width, float _height, const olc::Pixel& _colour, bool _visible_by_default){
    auto creation_data_rect = _scene->AddNewActorToMap<Rectangle>(_scene->rectangles, olc::vf2d(_x,_y),olc::vf2d(_width,_height));
    auto dependancy_node_rect = _scene->AddDependancy(_scene->GetDependancyTreeRoot(), creation_data_rect.id);
    auto creation_data_colour_rect = NewColourShape<Rectangle>(_scene, creation_data_rect.ptr, _colour, _visible_by_default);
    _scene->AddDependancy(dependancy_node_rect, creation_data_colour_rect.id);
    return creation_data_rect;
}

CreationData<Circle> NewCircle(Scene* _scene, float _x, float _y, float _radius, const olc::Pixel& _colour, bool _visible_by_default){
    auto creation_data_circle = _scene->AddNewActorToMap<Circle>(_scene->circles, olc::vf2d(_x,_y), _radius);
    auto dependancy_node_circle = _scene->AddDependancy(_scene->GetDependancyTreeRoot(), creation_data_circle.id);
    auto creation_data_colour_circle = NewColourShape<Circle>(_scene, creation_data_circle.ptr, _colour, _visible_by_default);
    _scene->AddDependancy(dependancy_node_circle, creation_data_colour_circle.id);
    return creation_data_circle;
}

CreationData<Ray2> NewLineSegment2(Scene* _scene, olc::vf2d _start, olc::vf2d _end, const olc::Pixel& _colour, bool _visible_by_default){
    auto creation_data_shape = _scene->AddNewActorToMap<Ray2>(_scene->line_segments, _start, _end);
    auto dependancy_node_shape = _scene->AddDependancy(_scene->GetDependancyTreeRoot(), creation_data_shape.id);
    auto creation_data_colour_shape = NewColourShape<Ray2>(_scene, creation_data_shape.ptr, _colour, _visible_by_default);
    _scene->AddDependancy(dependancy_node_shape, creation_data_colour_shape.id);
    return creation_data_shape;
}

CreationData<Ray2> NewUserRay2(Scene* _scene, olc::vf2d _start, olc::vf2d _end, const olc::Pixel& _colour, bool _visible_by_default){
    auto creation_data_shape = _scene->AddNewActorToMap<Ray2>(_scene->user_rays, _start, _end);
    auto dependancy_node_shape = _scene->AddDependancy(_scene->GetDependancyTreeRoot(), creation_data_shape.id);
    auto creation_data_colour_shape = NewColourShape<Ray2>(_scene, creation_data_shape.ptr, _colour, _visible_by_default);
    _scene->AddDependancy(dependancy_node_shape, creation_data_colour_shape.id);
    return creation_data_shape;
}