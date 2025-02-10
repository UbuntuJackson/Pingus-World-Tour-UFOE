#ifndef BALLOON
#define BALLOON

#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/shape.h"
#include "../json/json.h"

enum BalloonType{
    REGULAR,
    HEALTH,
    HAZARDOUS
};

class Balloon : public Shape<Circle>{
public:
    int balloon_type;
    olc::vf2d velocity = {0.0f, 0.0f};
    float count = 0.0f;
    Balloon(olc::vf2d _position, int _balloon_type);
    void OnLevelEnter(Level* _level);
    void OnUpdate();
    void OnPurge(Level* _level);
    static std::unique_ptr<Balloon> Load(Json& _json, int _balloon_type = BalloonType::REGULAR);
};

#endif