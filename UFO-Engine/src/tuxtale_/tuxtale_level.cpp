#include <memory>
#include "tuxtale_level.h"
#include "../level/level.h"
#include "tuxtale_player.h"
#include "../ufo_maths/ufo_maths.h"
#include "tuxtale_coin.h"
#include "../level/spawn_point.h"
#include "../animation/animated_sprite_reference.h"
#include "../animation/animation.h"
#include "grass_crawler.h"
#include "tuxtale_battle_hud.h"
#include "talkable.h"
#include "../level/level_trigger.h"
#include "tuxtale_heart.h"
#include "../file/file_menu.h"
#include "save_block.h"
#include "coin_block.h"
#include "../demonstration/custom_actor.h"
#include "controllable_ufo.h"
#include "info_block.h"

void TuxTaleLevel::OnResourceLoad(){
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Tux/tuxtale-animation/walk-down.png", "walk_down");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Tux/tuxtale-animation/walk-horizontal.png", "walk_horizontal");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Tux/tuxtale-animation/walk-up.png", "walk_up");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Tux/tuxtale-animation/tux_dying.png", "tux_dying");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/save_point_2p5_d.png", "save_point");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/bonus_block.png", "coin_block");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/shadow.png", "shadow");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/save_point_shadow.png", "save_point_shadow");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/tuxtale_coin.png", "tuxtale_coin");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/grass_crawler_anim-Sheet.png", "grass_crawler_anim");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/grass_crawler/grass_crawler_left.png", "grass_crawler_left");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/grass_crawler/new_grass_crawler_trail.png", "new_grass_crawler");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/grass_crawler/highlight.png", "highlight");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/attack.png", "attack");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/magic.png", "magic");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/defend.png", "defend");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/end.png", "end");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/act.png", "act");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/item.png", "item");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/green_maze_battle_arena.png", "flower_vs_tux");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/info_block.png", "info_block");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/grass_crawler_battle.png", "grass_crawler_battle");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/tuxtale_heart.png", "tuxtale_heart");
    AssetManager::Get().LoadAudio("../res/tuxtale/res/audio/bell.wav", "got_heart");
    AssetManager::Get().LoadAudio("../res/tuxtale/res/audio/bump.wav", "bump_head");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/Battle/tuxtale_sword.png", "tuxtale_sword");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/controllable_ufo-Sheet.png", "controllable_ufo");
    AssetManager::Get().LoadAudio("../res/tuxtale/res/audio/heal.wav", "ufo_dash");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/info_block_2p5d.png", "info_block_2p5d");
    AssetManager::Get().LoadDecal("../res/tuxtale/res/movable_pillar.png", "movable_pillar");

    Engine::Get().background_colour = olc::Pixel(0,0,0);

    NewActor<CustomActor>(Vector2f(0.0f, 0.0f));
}

void TuxTaleLevel::OnLoad(Json& _level_json){
    gravity_scale = 1.0f;
    Json properties = _level_json.GetObject("properties");
    Json::ArrayForEach(Json(),properties,[&](Json& _object, Json _objects){
        if(_object.GetAsString("name") == "gravity_scale"){
            gravity_scale = _object.GetAsDouble("value");
        }
    });
}

TileCollisionData TuxTaleLevel::PlaceFree(const Rectangle& _rectangle){
    int number_of_tiles = 0;

    int walking_speed_dt = int(Engine::Get().GetDeltaTime());

    std::vector<int> overlapped_tiles;
    bool place_free = true;

    for(int yy = (int)_rectangle.position.y/16 - (2); yy <= (int)_rectangle.position.y/16 + (2); yy++){
        for(int xx = (int)_rectangle.position.x/16 - (2+walking_speed_dt/16); xx <= (int)_rectangle.position.x/16 + (2+walking_speed_dt/16); xx++){
            TilesetData data = tilemap.GetTilesetData("collision_tiles");

            int tile_id = tilemap.tilemap_collision_data[xx+yy*tilemap.number_of_columns] - data.tileset_start_id + 1;

            if(tile_id != 0){
                if(ufoMaths::RectangleVsRectangle(_rectangle, Rectangle(Vector2f(xx*16.0f,yy*16.0f), Vector2f(16.0f,16.0f)))){
                    overlapped_tiles.push_back(tile_id);
                    if(tile_id == 1) place_free = false;
                }
            }

            number_of_tiles++;
        }
    }

    return TileCollisionData{place_free,overlapped_tiles};
}

TileCollisionData TuxTaleLevel::PlaceFree(const Circle& _rectangle){
    int number_of_tiles = 0;

    int walking_speed_dt = int(Engine::Get().GetDeltaTime());

    std::vector<int> overlapped_tiles;
    bool place_free = true;

    for(int yy = (int)_rectangle.position.y/16 - (2); yy <= (int)_rectangle.position.y/16 + (2); yy++){
        for(int xx = (int)_rectangle.position.x/16 - (2+walking_speed_dt/16); xx <= (int)_rectangle.position.x/16 + (2+walking_speed_dt/16); xx++){
            TilesetData data = tilemap.GetTilesetData("collision_tiles");

            int tile_id = tilemap.tilemap_collision_data[xx+yy*tilemap.number_of_columns] - data.tileset_start_id + 1;

            if(tile_id != 0){
                if(ufoMaths::RectangleVsCircle(Rectangle(Vector2f(xx*16.0f,yy*16.0f), Vector2f(16.0f,16.0f)),_rectangle)){
                    overlapped_tiles.push_back(tile_id);
                    if(tile_id == 1) place_free = false;
                }
            }

            number_of_tiles++;
        }
    }

    return TileCollisionData{place_free,overlapped_tiles};
}

TileCollisionData TuxTaleLevel::PlaceFreeControllableUFO(const Circle& _rectangle){
    int number_of_tiles = 0;

    int walking_speed_dt = int(Engine::Get().GetDeltaTime());

    std::vector<int> overlapped_tiles;
    bool place_free = true;

    for(int yy = (int)_rectangle.position.y/16 - (2); yy <= (int)_rectangle.position.y/16 + (2); yy++){
        for(int xx = (int)_rectangle.position.x/16 - (2+walking_speed_dt/16); xx <= (int)_rectangle.position.x/16 + (2+walking_speed_dt/16); xx++){
            TilesetData data = tilemap.GetTilesetData("collision_tiles");

            int tile_id = tilemap.tilemap_collision_data[xx+yy*tilemap.number_of_columns] - data.tileset_start_id + 1;

            
            if(ufoMaths::RectangleVsCircle(Rectangle(Vector2f(xx*16.0f,yy*16.0f), Vector2f(16.0f,16.0f)),_rectangle)){
                overlapped_tiles.push_back(tile_id);
                if(tile_id != 12 || tile_id == 13) place_free = false;
            }
            

            number_of_tiles++;
        }
    }

    return TileCollisionData{place_free,overlapped_tiles};
}

void TuxTaleLevel::OnLoadActors(Json& _actor_json){
    Console::Out("TuxTaleLevel::OnLoadActors");

    auto frogatto_actors = tilemap.GetTilesetData("objects");
    int type_id = _actor_json.GetAsInt("gid") - frogatto_actors.tileset_start_id+1;

    switch (type_id)
    {
    case 1:
        {
            auto u_info_block = Talkable::Load(_actor_json);

            NewActor(std::move(u_info_block));
        }
        break;
    case 2:
        {
            if(spawn_mode == SpawnMode::GAME_START) NewActor(TuxTalePlayer::Load(_actor_json));
            if(spawn_mode == SpawnMode::SPAWN_POINT){
                auto u_player = TuxTalePlayer::Load(_actor_json);
                u_player->local_position = spawn_point;
                NewActor(std::move(u_player));
            }
        }
        break;
    
    case 7:{
        auto save_point = NewActor<SaveBlock>(Vector2f(_actor_json.GetAsInt("x"),_actor_json.GetAsInt("y")-16.0f));

        }
        break;
    case 8:
        {
            auto u_coin = std::make_unique<TuxTaleCoin>(olc::vf2d((float)_actor_json.GetAsInt("x"), (float)_actor_json.GetAsInt("y")-16.0f));

            NewActor(std::move(u_coin));
        }
        break;
    case 9:
        {
            Engine::Get().wave_engine.StopAll();
            AssetManager::Get().LoadAudio(_actor_json.GetAsString("name"), _actor_json.GetAsString("name"));
            AssetManager::Get().PlayAudio(_actor_json.GetAsString("name"), true);
            AssetManager::Get().current_music_track = _actor_json.GetAsString("name");
        }
        break;
    case 10:
        {
            NewActor(GrassCrawler::Load(_actor_json));

        }
        break;
    
    case 11:
        {
            NewActor(std::make_unique<TuxTaleHud>());
            NewActor(std::make_unique<SpriteReference>(
                "flower_vs_tux",
                Vector2f(0.0f, 0.0f),
                Vector2f(0.0f, 0.0f),
                Vector2f(340.0f, 240.0f),
                Vector2f(1.0f, 1.0f),
                0.0f,
                0
            ));
        }
        break;

    case 12:
        {
            NewActor<TuxTaleHeart>(Vector2f(_actor_json.GetAsInt("x"), _actor_json.GetAsInt("y")-16.0f));
        }
        break;
    case 13:
        {
            NewActor<CoinBlock>(Vector2f(_actor_json.GetAsInt("x"),_actor_json.GetAsInt("y")-16.0f));
        }
        break;
    case 14:
        {
            NewActor(ControllableUFO::Load(&_actor_json));
        }
        break;
    case 17:
        {
            Console::Out("Actor type index: 15");
            NewActor(InfoBlock::Load(&_actor_json));
        }
        break;

    default:
        break;
    }
}

void TuxTaleLevel::OnLoadLayer(Json& _layer_json){
    if(_layer_json.GetAsString("name") == "tilemap_collision"){
        Console::Out("found tilemap collision");
        Json::ArrayForEach( Json(), _layer_json.GetObject("data"), [&](Json _json_a, Json _data_a){
            
            tilemap.tilemap_collision_data.push_back(_json_a.GetAsInt());
        });
    }
}

void TuxTaleLevel::OnExit(){
    Console::Out("on Exit");

    Json& current_save_file = Engine::Get().GetActiveProfile()->save_file;

    if(current_save_file.GetObject("visited_levels").IsNull()){
        current_save_file.PushToObject("visited_levels", Json::NewObject());
    }

    Json visited_levels = current_save_file.GetObject("visited_levels");

    Json dead_actors = Json::NewArray();

    for(int i = 0; i < 10; i++){
        dead_actors.PushToArray(Json::NewObject(-1));
    }

    Json level_data = Json::NewObject();

    level_data.Set("dead_actors", dead_actors);

    visited_levels.Set(path,level_data);

    current_save_file.Set("visited_levels", visited_levels);


}

void TuxTaleLevel::OnUpdate(){
    Level::OnUpdate();

    if(debug_menu){
        debug_menu->OnUpdate();
        if(SingleKeyboard::Get().GetKey(olc::F1).is_pressed){
            debug_menu->QueueForPurge();
            debug_menu = nullptr;
            paused = false;
        }
    }

    if(!debug_menu){
        if(SingleKeyboard::Get().GetKey(olc::F1).is_pressed){
            
            paused = true;
            //auto u_sub_window = std::make_unique<SubWindow>(Vector2f(20.0f, 20.0f),Vector2f(400.0f, 400.0f));

            auto u_level_select_menu = std::make_unique<FileMenu>(Vector2f(20.0f, 20.0f),Vector2f(250.0f, 20.0f),"../res");

            u_level_select_menu->func_on_file_menu_created = [](Widget* _w){
                Console::Out("File menu:",dynamic_cast<FileMenu*>(_w));
                dynamic_cast<TuxTaleLevel*>(_w->level)->debug_menu = dynamic_cast<FileMenu*>(_w);
            };

            u_level_select_menu->func_on_file_open = [](Widget* _w, FileMenuButton* _button, std::string _file_extension){
                if(_file_extension == "json"){
                    Engine::Get().GoToLevel(_button->path);
                }
                else{
                    Console::Out("File is not .json");
                }

                if(_file_extension == "cpp"){
                    Console::Out(File().Read(_button->path));

                    _w->level->NewActor(std::make_unique<Label>(Vector2f(400.0f, 0.0f), Vector2f(400.0f, 400.0f), File().Read(_button->path)));
                }
            };

            //u_sub_window->AddChild((std::move(u_level_select_menu)));
            debug_menu = u_level_select_menu.get();

            Console::Out("Debug menu:", debug_menu);

            NewActor(std::move(u_level_select_menu));
        }
    }

}

void TuxTaleLevel::OnHandlePurge(){
    PurgeHandles(collectable_handles);
    PurgeHandles(enemy_handles);
    PurgeHandles(savable_actor_handles);
}

void TuxTaleLevel::OnSave(Json* _current_save){

    _current_save->Set("level_path", Json::NewObject(path));
    
}

void TuxTaleLevel::OnSaveFileLoad(Json* _current_save){

    Console::Out("OnSaveFileLoad:",_current_save->IsNull());

    Json player_data = _current_save->GetObject("player_data");

    float player_x = (float)player_data.GetAsDouble("x");
    float player_y = (float)player_data.GetAsDouble("y");
    spawn_point = {player_x, player_y};

    auto u_player = std::make_unique<TuxTalePlayer>(Vector2f(player_x, player_y));
    
    NewActor(std::move(u_player));
}