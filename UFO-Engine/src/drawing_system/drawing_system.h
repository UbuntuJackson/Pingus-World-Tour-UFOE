#ifndef RENDERING_SYSTEM
#define RENDERING_SYSTEM
#include <unordered_map>
#include "../ufo_sprite/ufo_sprite.h"
#include "../camera/camera.h"
#include "../graphics_provider/graphics_provider.h"
#include "../asset_manager/asset_manager.h"
#include "../shapes/circle.h"
#include "../camera/transformations.h"
#include "../sprite_reference/sprite_reference.h"
#include "../shapes/pill.h"

class Scene;

class DrawingSystem{
public:
    DrawingSystem() = default;
    
    std::vector<std::pair<Circle*, bool>> circles;
    std::vector<std::pair<Rectangle*, bool>> rectangles;
    std::vector<std::pair<ufoSprite*, bool>> ufo_sprites;

    void DrawRectangles();
    void Update();

    template<typename tType>
    void DrawItemsInContainer(std::unordered_map<int, tType> _map){
        for(const auto& [key,item] : _map){
            Draw(*(item.get()));
        }
    }

    static void Draw(const Circle& _circle, const olc::Pixel& _colour);
    static void Draw(const Circle& _circle, Camera* _camera, const olc::Pixel& _colour);

    static void DrawFilled(const Rectangle& _rectangle, const olc::Pixel& _colour);
    static void DrawFilled(const Rectangle& _rectangle, Camera* _camera, const olc::Pixel& _colour);

    static void Draw(const Rectangle& _circle, const olc::Pixel& _colour);
    static void Draw(const Rectangle& _circle, Camera* _camera, const olc::Pixel& _colour);

    static void Draw(const Ray2& _ray, const olc::Pixel& _colour);
    static void Draw(const Ray2& _ray, Camera* _camera, const olc::Pixel& _colour);

    static void Draw(const Pill& _pill, const olc::Pixel& _colour);
    static void Draw(const Pill& _pill, Camera* _camera, const olc::Pixel& _colour);

    static void Draw(const olc::vf2d& _point, const olc::Pixel& _colour);
    static void Draw(const olc::vf2d& _point, Camera* _camera, const olc::Pixel& _colour);

    static void Draw(const SpriteReference& _ufo_sprite);

    static void Draw(const SpriteReference& _ufo_sprite, Camera* _camera);

    static void DrawString(const olc::vf2d& _position,const std::string& _s, const olc::Pixel& _colour, const olc::vf2d& _scale);
};

#endif