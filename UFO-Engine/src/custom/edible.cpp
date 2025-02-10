#include "edible.h"
#include "../level/actor.h"
#include "../level/level.h"
#include "frogatto_level.h"

Edible::Edible() : Actor(olc::vf2d(0.0f,0.0f)){}

bool Edible::CheckOverlap(RawShapeBase* _shape){}

void Edible::OnEaten(Frogatto* _frogatto){}