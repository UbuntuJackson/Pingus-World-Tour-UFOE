#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include "../shapes/rectangle.h"
#include "../ufo_maths/ufo_maths.h"
#include "../external/olcPixelGameEngine.h"
#include "../mouse/mouse.h"
#include "button.h"
#include "menu_system.h"
#include "../console/console.h"
#include "../graphics_provider/graphics_provider.h"
#include "../camera/camera.h"
#include "../scene/scene.h"
#include "../scene_system/scene_system.h"
#include "../ufo_lang/ufo_lang.h"

void MenuSystem::LoadWidgets(){}

bool MenuSystem::Clicked(Rectangle _rectangle, olc::vf2d _position){
    if(ufoMaths::RectangleVsPoint(_rectangle, _position) && Mouse::GetLeftButton().is_pressed){
        return true;
    }
    return false;
}

bool MenuSystem::Held(Rectangle _rectangle){
    if(ufoMaths::RectangleVsPoint(_rectangle, Mouse::GetPosition()) && Mouse::GetLeftButton().is_pressed){
        return true;
    }
    return false;
}

bool MenuSystem::HoveredOver(Rectangle _rectangle){
    if(ufoMaths::RectangleVsPoint(_rectangle, Mouse::GetPosition())){
        return true;
    }
    return false;
}

void MenuSystem::Update(std::unordered_map<int, std::unique_ptr<Button>> *_buttons, Camera* _camera){
    for(auto&& [k, button] : *_buttons){
        if(button->on_clicked == "") continue;
        if(Clicked(button->rectangle, _camera->TransformScreenToWorld(Mouse::GetPosition()))){
            ufoLang::Execute(scene, button->on_clicked);
        }
    }
}

void MenuSystem::Draw(std::unordered_map<int, std::unique_ptr<Button>> *_buttons, Camera* _camera){
    for(const auto& [k, button] : *_buttons){
        Rectangle button_rect = Rectangle(_camera->Transform(button->rectangle.position), _camera->scale*button->rectangle.size);
        if(ufoMaths::RectangleVsPoint(button->rectangle, _camera->TransformScreenToWorld(Mouse::GetPosition()))){
            GraphicsProvider::Get().FillRectDecal(button_rect.position, button_rect.size, olc::Pixel(210,140,0));
        }
        else{
            GraphicsProvider::Get().FillRectDecal(button_rect.position, button_rect.size, olc::Pixel(230,240,0));
        }
        int assumed_character_width = 8;
        int assumed_character_height = 8;

        int length_of_text_half = button->text.size()*8/2;
        float draw_x = button->rectangle.position.x + button->rectangle.size.x/2.0f - length_of_text_half;
        float draw_y = button->rectangle.position.y + button->rectangle.size.y/2.0f - assumed_character_height/2.0f;

        float default_scale = 1.0f;
        //Console::Out(button->text);
        //Console::Out("drawx, drawy:",_camera->Transform({draw_x, draw_y}));

        GraphicsProvider::Get().DrawStringDecal(_camera->Transform({draw_x, draw_y}), button->text, olc::RED, {_camera->scale*default_scale, _camera->scale*default_scale});
    }
}