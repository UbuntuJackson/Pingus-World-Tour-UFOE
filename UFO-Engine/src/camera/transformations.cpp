#include "../external/olcPixelGameEngine.h"
#include "../shapes/circle.h"
#include "../shapes/rectangle.h"
#include "../ufo_sprite/ufo_sprite.h"
#include "camera.h"
#include "../shapes/ray2.h"
#include "../sprite_reference/sprite_reference.h"

SpriteReference Transform_ufoSprite(SpriteReference *_sprite, Camera *_camera){
    SpriteReference transformed_sprite(
        _sprite->key,
        _camera->Transform(_sprite->position),
        _sprite->offset,
        _sprite->frame_size,
        _camera->scale * _sprite->scale,
        _sprite->rotation,
        _sprite->z_index
    );
    return transformed_sprite;
}

Circle TransformCircle(Circle *_circle, Camera *_camera){
    Circle transformed_circle{_camera->Transform(_circle->position), _circle->radius * _camera->scale};
    return transformed_circle;
}

ufo::Rectangle TransformRectangle(ufo::Rectangle *_rectangle, Camera *_camera){
    ufo::Rectangle transformed_rectangle{
        _camera->Transform(_rectangle->position),
        _rectangle->size * _camera->scale};
    return transformed_rectangle;
}

Ray2 TransformRay2(Ray2 *_ray, Camera *_camera){
    return Ray2(
        _camera->Transform(_ray->Start()),
        _camera->Transform(_ray->End())
    );
}