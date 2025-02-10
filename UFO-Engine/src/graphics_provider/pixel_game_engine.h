#ifndef UFO_PGE_SIMPLIFIED_H
#define UFO_PGE_SIMPLIFIED_H
#include <string>
#include "../external/olcPixelGameEngine.h"

class Engine;

class PixelGameEngine : public olc::PixelGameEngine{
public:
    Engine* engine;
    PixelGameEngine();
    olc::vi2d GetWindowSizeInPixles();
    virtual bool OnUserCreate();
    virtual bool OnUserUpdate(float fElapsedTime);
    virtual bool OnUserCommand(std::string& _text);
};

#endif