#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <sprite_reference.h>
#include <mouse.h>

class Level;
class PingusLevel;
class SpriteReference;

/// @spawn;
class WorldMapCursor : public Actor{
public:
    SpriteReference* spr = nullptr;
    PingusLevel* level = nullptr;
    WorldMapCursor(Vector2f _) : Actor(_){}

    void OnLevelEnter(Level* _level);

    void OnUpdate();

};