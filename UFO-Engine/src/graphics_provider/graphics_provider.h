#ifndef GRAPHICS_PROVIDER_H
#define GRAPHICS_PROVIDER_H
#include "ufo_pge.h"
class Camera;

class GraphicsProvider{
public:
    static UFO_PixelGameEngine& Get(){
        static UFO_PixelGameEngine ufo_pge;
        return ufo_pge;
    }
};

#endif