#pragma once

#include <vector>
#include <string>
#include <actor.h>
#include <level.h>
#include <sprite_reference.h>
#include <one_dimensional_text_box.h>
#include <algorithm>

class StoryModeIntro : public Actor{
public:

    OneDimensionalTextBox* text_box = nullptr;

    SpriteReference* image = nullptr;

    StoryModeIntro(Vector2f _local_position) : Actor(_local_position){

    }

    void OnLevelEnter(Level* _level){
        text_box = _level->NewActor<OneDimensionalTextBox>(Vector2f(10.0f, 200.0f),Vector2f(500.0f, 200.0f));

        text_box->text_queue.push_back("These are the wild pingus in their natural habitat.");
        text_box->text_queue.push_back("The pingus are small, penguin-like creatures found on the shores of Antarctica. Where they live in harmony in large groups.");
        text_box->text_queue.push_back("Or so they used to...");
        text_box->text_queue.push_back("One day things started to change. The climate got hotter. The icesheets melted away. Chaos ensued.");
        text_box->text_queue.push_back("The pingus assembled and trained a team of pingus to set out into the world to find out what was happening.");
        text_box->text_queue.push_back("They are off to a good start, but something is missing. They are too prone to walk off edges...");
        text_box->text_queue.push_back("What they need is leadership, and it is upto YOU, the player to guide them on their grand new WORLD TOUR.");
        text_box->text_queue.push_back("~ The Pingus' World Tour ~ \n (c) UFO-Engine Studios 2025");
        
        std::reverse(text_box->text_queue.begin(), text_box->text_queue.end());

    }

};