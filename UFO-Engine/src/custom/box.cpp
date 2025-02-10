#include <memory>
#include "box.h"
#include "../external/olcPixelGameEngine.h"
#include "../shapes/rectangle.h"
#include "../shapes/shape.h"
#include "../json/json.h"
#include "../level/level.h"
#include "../sprite_reference/sprite_reference.h"
#include "edible.h"
#include "frogatto.h"

Box::Box(olc::vf2d _position) : Shape<Rectangle>(Rectangle(olc::vf2d(0.0f, 0.0f), olc::vf2d(8.0f, 8.0f)),_position){}

void Box::OnLevelEnter(Level* _level){
    AddChild(std::make_unique<SpriteReference>(
        "small_box",
        olc::vf2d(0.0f,0.0f),
        olc::vf2d(0.0f, 0.0f),
        olc::vf2d(12.0f, 12.0f),
        olc::vf2d(1.0f, 1.0f),
        0.0f,
        0
    ));

    AddChild(std::make_unique<TEdible<Box>>(this, &shape));
}

void Box::OnEaten(Frogatto* _frogatto){
    _frogatto->eaten_actor_spawner = [](Frogatto* _ffrogatto){
        auto new_box = std::make_unique<Box>(_ffrogatto->position+olc::vf2d(8.0f*_ffrogatto->face_direction, -1.0f));
        return std::move(new_box);
    };
}

std::unique_ptr<Box> Box::Load(Json& _json){
    return std::make_unique<Box>(olc::vf2d(_json.GetAsInt("x"), _json.GetAsInt("y")));
}