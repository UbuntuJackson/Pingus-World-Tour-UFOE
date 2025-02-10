#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include "../shapes/rectangle.h"
#include "../ufo_maths/ufo_maths.h"
#include "../mouse/mouse.h"
#include "button.h"
#include "../console/console.h"
class Camera;
class Scene;

struct MenuCommand{
    std::string command;
    std::vector<std::string> args;
};

class MenuSystem{
public:
    std::unordered_map<std::string, std::string> config;
    Scene* scene;

    MenuSystem() = default;

    void LoadWidgets();
    /*
    void ClearSystem(){
        buttons.clear();
        texts.clear();
        menu_frames.clear();
        sliders.clear();
    }
    */
    bool Clicked(Rectangle _rectangle, olc::vf2d _position);

    bool Held(Rectangle _rectangle);

    bool HoveredOver(Rectangle _rectangle);

    void Update(std::unordered_map<int, std::unique_ptr<Button>> *_buttons, Camera* _camera);
    void Draw(std::unordered_map<int, std::unique_ptr<Button>> *_buttons, Camera* _camera);
};

#endif