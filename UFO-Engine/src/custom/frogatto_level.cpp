#include <memory>
#include "edible.h"
#include "frogatto_level.h"
#include "../animation/animation.h"
#include "../level/level.h"
#include "../json/json.h"
#include "../custom/balloon.h"
#include "../custom/ant.h"
#include "../custom/frogatto.h"
#include "../asset_manager/asset_manager.h"
#include "../shapes/shape.h"
#include "../shapes/circle.h"
#include "heart_bubble.h"
#include "box.h"
//#include "../garbage_collected/garbage_collected.h"
#include "../background/background_image.h"
#include "water_body.h"
#include "../camera/camera.h"

FrogattoLevel::FrogattoLevel(){
    level_class = "FrogattoLevel";
    retain_player_properties = true;
    Console::Out("[!] Level class:", level_class);
}

void FrogattoLevel::OnLoadActors(Json& _actor_json){
    auto frogatto_actors = tilemap.GetTilesetData("frogatto_actors");
    int type_id = _actor_json.GetAsInt("gid") - frogatto_actors.tileset_start_id+1;

    switch(type_id){
        //case 1:
        //    NewActor(SpawnPoint::Load(_object));
        //    break;
        case 2:
            {
                auto u_coin_animation = std::make_unique<Animation>(olc::vf2d(-6.0f, -6.0f));

                u_coin_animation->AddAnimationState(AnimatedSpriteReference(
                    "coin",
                    olc::vf2d(0.0f, 0.0f),
                    olc::vf2d(0.0f,0.0f),
                    olc::vf2d(12.0f,12.0f),
                    olc::vf2d(1.0f, 1.0f),
                    0.0f,
                    0,
                    5.0f
                ));
                u_coin_animation->SetAnimation("coin");

                std::unique_ptr<Shape<Circle>> u_coin_shape = std::make_unique<Shape<Circle>>(
                    Circle(olc::vf2d(0.0f, 0.0f), 6.0f), olc::vf2d((float)_actor_json.GetAsInt("x"), (float)_actor_json.GetAsInt("y")));

                coin_handles.push_back(u_coin_shape.get());
                u_coin_shape->AddChild(std::move(u_coin_animation));
                NewActor(std::move(u_coin_shape));
            }
            break;
        case 3:
            NewActor(Frogatto::Load(_actor_json, this));
            break;
        case 4:
            NewActor(Balloon::Load(_actor_json));
            break;
        case 5:
            NewActor(Ant::Load(_actor_json));
            break;
        case 6:
            NewActor(Balloon::Load(_actor_json, BalloonType::HEALTH));
            break;
        case 7:
            NewActor(Balloon::Load(_actor_json, BalloonType::HAZARDOUS));
            break;
        case 8:
            /*{
            std::unique_ptr<GarbageCollected<SpriteReference>> u_gc_sprite = MakeGarbageCollected<SpriteReference>(
                "placeholder",
                olc::vf2d(_actor_json.GetAsInt("x"), _actor_json.GetAsInt("y")),
                olc::vf2d(0.0f, 0.0f),
                olc::vf2d(16.0f, 16.0f),
                olc::vf2d(1.0f, 1.0f),
                0.0f, 0
            );
            //doesnt go out of scope here.

            gc_sprite = *u_gc_sprite.get();
            gc_sprite2 = *u_gc_sprite.get();

            NewActor(std::move(u_gc_sprite));

            }*/
            break;
        case 9:
            NewActor(Box::Load(_actor_json));
            break;
    }
}

void FrogattoLevel::OnLoadLayer(Json& _layer_json){
    if(_layer_json.GetAsString("name") == "water"){

        Json::ArrayForEach( Json(), _layer_json.GetObject("objects"), [&](Json _json_a, Json _data_a){
            NewActor(WaterBody::Load(_json_a));
        });
    }
}

void
FrogattoLevel::OnResourceLoad(){
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_idle.png", "frogatto_idle");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_hurt.png", "frogatto_hurt");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogattowalk.png", "frogatto_walk");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogattojump.png", "frogatto_jump");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_tongue_attack.png", "frogatto_tongue_attack");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/tongue.png", "tongue");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/baloon.png", "baloon");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/coin.png", "coin");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/heart.png", "frogatto_heart");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_magic_meter_background.png", "frogatto_magic_meter_background");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_magic_meter_foreground.png", "frogatto_magic_meter_foreground");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/ant_animation-Sheet.png", "blue_ant");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/ant_animation_yellow-Sheet.png", "yellow_ant");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/test_bg.png", "test_bg");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/coin_sparkle.png", "coin_sparkle");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/heart_bubble_sparkles.png", "heart_sparkle");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/heart_bubble_sparkle.png", "heart_bubble_sparkle");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/heart_bubble.png", "heart_bubble");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/small_box.png", "small_box");
    AssetManager::Get().LoadDecal("../res/engine/placeholder.png", "placeholder");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_water.png", "water");
    AssetManager::Get().LoadDecal("../res/frogatto_jr/frogatto_under_water.png", "under_water");
    auto result = Engine::Get().pixel_game_engine.LoadFontSprite("../res/frogatto_jr/frogatto_jr_font.png");
    if(result == olc::rcode::NO_FILE) Console::Out("no file");

    /*AssetManager::Get().LoadDecal("../res/frogatto_jr/Anever.png","Anever");
    NewActor(std::make_unique<BackgroundImage>(
        "test_bg",
        olc::vf2d(0.0f,0.0f),
        olc::vf2d(0.0f,0.0f),
        olc::vf2d(700.0f, 250.0f),
        olc::vf2d(1.0f, 1.0f),
        0.0f, 1.0f, olc::vf2d(0.8f, 0.5f), 0));*/
}

void
FrogattoLevel::OnUpdate(){
    
    //TestPassGarbageCollected(gc_sprite);
    //if(gc_sprite.Get() != nullptr) Console::Out(gc_sprite.Get()->reference_count);

    Level::OnUpdate();
}

void
FrogattoLevel::OnHandlePurge(){
    PurgeHandles(balloon_handles);
    PurgeHandles(coin_handles);
    PurgeHandles(heart_bubble_handles);
    PurgeHandles(enemy_handles);
    PurgeHandles(edible_handles);
}

void
FrogattoLevel::OnBackgroundElementDraw(Camera* _camera){
    for(const auto& water_body : water_body_handles){
        water_body->DrawWater(_camera);
    }
}