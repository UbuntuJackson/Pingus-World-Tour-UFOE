#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include "../external/olcPixelGameEngine.h"
#include "../shapes/circle.h"
#include "../shapes/rectangle.h"
#include "../shapes/ray2.h"
#include "../sprite_reference/sprite_reference.h"
class Camera;
class ufoSprite;

SpriteReference Transform_ufoSprite(SpriteReference* _sprite, Camera *_camera);

Circle TransformCircle(Circle *_circle, Camera *_camera);

Rectangle TransformRectangle(Rectangle *_rectangle, Camera *_camera);

Ray2 TransformRay2(Ray2 *_ray, Camera *_camera);

#endif