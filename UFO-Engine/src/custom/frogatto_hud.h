#ifndef FROGATTO_HUD
#define FROGATTO_HUD

#include <vector>
#include <string>
#include "../level/actor.h"
#include "../sprite_reference/widget_sprite_reference.h"
#include "../external/olcPixelGameEngine.h"

enum Abilities{
    TONGUE,
    FIRE,
    ZAP,
    RUSH
};

class FrogattoHud : public Actor{
public:
    std::vector<WidgetSpriteReference*> heart_handles;

    WidgetSpriteReference* hud_background_sprite_reference;

    int magic = 40;
    int coins = 0;

    int current_ability = TONGUE;
    int ability_index = 0;
    std::vector<int> unlocked_abilities;

    FrogattoHud() : Actor(olc::vf2d(0.0f, 0.0f)){
        unlocked_abilities.push_back(Abilities::TONGUE);
        unlocked_abilities.push_back(Abilities::FIRE);
        unlocked_abilities.push_back(Abilities::ZAP);
        unlocked_abilities.push_back(Abilities::RUSH);

    }
    void OnLevelEnter(Level* _level){
        for(int i = 0; i < 4; i++){
            auto spr = std::make_unique<WidgetSpriteReference>(
                "frogatto_heart",
                olc::vf2d(2.0f+float(i)*14.0f, 20.0f+4.0f),
                olc::vf2d(0.0f, 0.0f),
                olc::vf2d(12.0f, 11.0f),
                olc::vf2d(1.0f, 1.0f),
                0.0f, 0
            );
            heart_handles.push_back(spr.get());
            AddChild(std::move(spr));
        }

        auto u_hud_background_sprite_reference = std::make_unique<WidgetSpriteReference>(
            "frogatto_magic_meter_background",
            olc::vf2d(2.0f, 2.0f),
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(81.0f, 20.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0
        );

        hud_background_sprite_reference = u_hud_background_sprite_reference.get();

        AddChild(
            std::move(u_hud_background_sprite_reference)
        );
        
        /*spr_magic_meter_foreground = std::make_unique<WidgetSpriteReference>(
            "frogatto_magic_meter_background",
            olc::vf2d(2.0f, 2.0f),
            olc::vf2d(0.0f, 0.0f),
            olc::vf2d(1.0f, 6.0f),
            olc::vf2d(1.0f, 1.0f),
            0.0f, 0
        );*/
    }

    void SwitchAbility(){
        ability_index++;
        ability_index = ability_index%unlocked_abilities.size();
        current_ability = unlocked_abilities[ability_index];

        hud_background_sprite_reference->current_frame_index = ability_index;

    }

    void SetHealth(int _number){
        for(int i = 0; i < heart_handles.size(); i++){
            if(i < _number) heart_handles[i]->current_frame_index = 0;
            else heart_handles[i]->current_frame_index = 1;
        }
    }
    void SetMagic(int _number){
        //...
    }
    void OnWidgetDraw(){
        for(int i = 0; i < magic; i++){
            auto spr = WidgetSpriteReference(
                "frogatto_magic_meter_foreground",
                olc::vf2d(2.0f+(float)i+24.0f, 2.0f+11.0f),
                olc::vf2d(0.0f, 0.0f),
                olc::vf2d(1.0f, 6.0f),
                olc::vf2d(1.0f, 1.0f),
                0.0f, 0, true
            );

            int low_magic_offset = 4;
            if(magic < 4){low_magic_offset = magic;}
            if(i > magic-low_magic_offset){
                spr.current_frame_index = (54-(magic-i))+54*ability_index;
            }
            else{
                spr.current_frame_index = i + 54*ability_index;
            }
            spr.position = spr.local_position;
            DrawingSystem::Draw(spr);
        }

        Engine::Get().pixel_game_engine.DrawStringDecal(olc::vf2d(2.0f, 38.0f), "Coins x " + std::to_string(coins));
        
    }

};

#endif