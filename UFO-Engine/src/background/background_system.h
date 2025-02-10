#ifndef BACKGROUND_SYSTEM_H
#define BACKGROUND_SYSTEM_H
#include <vector>
#include "../shapes/rectangle.h"
#include "../camera/camera.h"
#include "../external/olcPixelGameEngine.h"

struct BackgroundData{
    std::string key;
    olc::vf2d parallax;
    Rectangle rectangle;
};

class BackgroundSystem{
public:
    BackgroundSystem() = default;
    std::vector<BackgroundData> backgrounds;
    void HandleUpdate(Camera* _camera);
};

#endif