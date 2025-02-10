#include "../shapes/circle.h"
#include "../shapes/rectangle.h"
#include "../ufo_sprite/ufo_sprite.h"
#include "../external/olcPixelGameEngine.h"
#include "../camera/camera.h"
#include "../graphics_provider/graphics_provider.h"

namespace ufo{

void Draw(const Circle& _circle, const olc::Pixel& _colour){
    GraphicsProvider::Get().DrawCircle(_circle.position, _circle.radius);
}

void Draw(const Circle& _circle, Camera* _camera, const olc::Pixel& _colour){
    Draw(_camera->Transform(_circle));
}

void Draw(const Rectangle& _rectangle, const olc::Pixel& _colour){
    GraphicsProvider::Get().DrawRect(_circle.position, _circle.radius);
}

void Draw(const Rectangle& _rectangle, Camera* _camera, const olc::Pixel& _colour){
    Draw(_camera->Transform(_rectangle));
}

void Draw(const olc::vf2d& _point){

}

void Draw(const olc::vf2d& _point, Camera* _camera){

}

void Draw(const ufoSprite& _ufo_sprite){

}

void Draw(const ufoSprite& _ufo_sprite, Camera* _camera){
    
}
}