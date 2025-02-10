#ifndef UFO_PGE_H
#define UFO_PGE_H
#include "../external/olcPixelGameEngine.h"

class UFO_EngineMain;

class UFO_PixelGameEngine : public olc::PixelGameEngine{
public:
    UFO_EngineMain* ufo_engine_main;
    UFO_PixelGameEngine();
    bool OnUserCreate();
    bool OnUserUpdate(float fElapsedTime);
    virtual bool OnUserCommand(std::string& _text);
};

#endif