#ifndef GRASS_CRAWLER_H
#define GRASS_CRAWLER_H

#include <memory>
#include "../forward/actor_forward.h"
#include "../json/json.h"
#include "enemy.h"

class Animation;
class TuxTalePlayer;
class TuxTaleLevel;

using namespace ufoMaths;

class GrassCrawler : public Enemy{
public:
    Animation* animation;
    TuxTaleLevel* level;
    Vector2f direction = {-16.0f, 0.0f};
    std::string battle_arena_path = "../res/tuxtale/res/tiles/tux_vs_flower.json";

    float speed = 10.0f;

    bool clockwise = false;

    GrassCrawler(Vector2f _position);

    void OnLevelEnter(Level* _level);

    void OnUpdate();

    bool FoundPath(const Rectangle& _rectangle);

    void OnHit(TuxTalePlayer* _player);

    static std::unique_ptr<GrassCrawler> Load(Json& _json);
};

#endif