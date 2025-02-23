#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include "level.h"
#include "../console/console.h"
#include "../sprite_reference/sprite_reference.h"
#include "../asset_manager/asset_manager.h"
#include "../external/olcPixelGameEngine.h"
#include "../mouse/mouse.h"
#include "../shapes/shape.h"
#include "../shapes/rectangle.h"
#include "../json/json.h"
#include "../json/json_variant.h"
#include "../tilemap/tileset_data.h"
#include "../ufo_benchmarker/ufo_benchmarker.h"
#include "../custom/button_view_cameras.h"
#include "../custom/button_view_sprite_references.h"
#include "../ufo_vector_utils/find_in_vector.h"
#include "../shapes/circle.h"
#include "tcollision_body.h"
#include "../custom/button_view_actors.h"
#include "../custom/test_collision_body.h"
#include "../background/background_image.h"
#include "../animation/animated_sprite_reference.h"
#include "level_trigger.h"
#include "trigger.h"
#include "spawn_point.h"
#include "../player_body/player_body.h"
#include "level_transition_effect.h"
#include "../tilemap/tile_map.h"
#include "../sprite_reference/level_sprite_reference.h"

void Level::NewActor(std::unique_ptr<Actor> _actor){
    if(_actor->id == -1){
        Console::Out("[!] warning at Level::NewActor. id was not properly assigned.\n Make sure to call constructor of class Actor");
    }
    _actor->LevelEnter(this);
    new_actor_queue.push_back(std::move(_actor));
}

void Level::OnLoadJsonCustom(Json& _json){

}

void Level::Load(){

    Console::Out("loaded a level");

    //Loads all level attributes from a json file. Said file path is specified in Engine::GoToLevel()

    ufoBenchMarker benchmarker;

    bool found_actor_layer = false;

    pixel_effect_sprite = olc::Sprite(Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x, Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y);
    //empty_sprite = olc::Sprite(Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x, Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y);
    pixel_effect_decal = std::make_unique<olc::Decal>(&pixel_effect_sprite);

    for(int y = 0; y < Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y; y++){
        for(int x = 0; x < Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x; x++){
            pixel_effect_sprite.SetPixel(x,y, olc::Pixel(0,0,0,0));
        }
    }

    AssetManager::Get().LoadDecal("../UFO-Engine/res/engine/camera_icon.png", "camera_icon");
    AssetManager::Get().LoadDecal("../UFO-Engine/res/engine/filmroll.png", "filmroll");
    AssetManager::Get().LoadDecal("../UFO-Engine/res/engine/character.png", "character");
    
    OnResourceLoad();

    if(level_format == Engine::LevelFormat::JSON_CUSTOM){
        Json custom_json_level = Json::Read(path);

        OnLoadJsonCustom(custom_json_level);

        path = "../UFO-Engine/res/engine/default_level.json";
    }

    JsonDictionary level_json = JsonVariant::Read(path);

    //Getting the relative path, which has to be manually specified for now. It's unfortunate.
    std::string relative_path = "";
    
    Console::Out(path);

    if(level_json.KeyExists("properties")){
        JsonArray& properties = level_json.Get("properties").AsArray();
        
        for(auto&& property_json : properties.Iterable()){
            Console::Out("Is it an array?",property_json->IsDictionary());

            std::string name = property_json->AsDictionary().Get("name").AsString();
            if(name == "relative path") relative_path = property_json->AsDictionary().Get("value").AsString();
            Console::Out("[!!!]", "Using relative path:", relative_path, ".", "Should be resolves asap.");
        }
    }

    //This section handles all the layers.
    int tile_size_x = level_json.Get("tilewidth").AsInt();
    int tile_size_y = level_json.Get("tileheight").AsInt();
    int map_size_x = level_json.Get("width").AsInt();
    int map_size_y = level_json.Get("height").AsInt();

    level_size = {(float)tile_size_x*map_size_x,(float)tile_size_y*map_size_y};

    //Global camera
    NewActor(std::make_unique<Camera>(olc::vf2d(0.0f, 0.0f)));
    
    tilemap.number_of_columns = map_size_x;
    tilemap.number_of_rows = map_size_y;
    tilemap.tile_width = tile_size_x;
    tilemap.tile_height = tile_size_y;
    
    Console::Out("[!]", "Currently loading tilesets");

    for(auto&& tileset_json : level_json.Get("tilesets").AsArray().Iterable()){
        auto tileset_json_dictionary = tileset_json->AsDictionary();
        
        std::string image = tileset_json_dictionary.Get("image").AsString();
        std::string name = tileset_json_dictionary.Get("name").AsString();

        std::string image_path = relative_path;

        if(name == "actors"){image_path = "../UFO-Engine/res/engine"; image = "actors.png";}

        AssetManager::Get().LoadDecal(image_path+"/"+image, name);
        tilemap.keys_to_temporary_assets.push_back(name);

        int columns = tileset_json_dictionary.Get("columns").AsInt();
        int first_gid = tileset_json_dictionary.Get("firstgid").AsInt();
        int image_height = tileset_json_dictionary.Get("imageheight").AsInt();
        int image_width = tileset_json_dictionary.Get("imagewidth").AsInt();
        int tile_count = tileset_json_dictionary.Get("tilecount").AsInt();
        int tileset_tile_size_x = tileset_json_dictionary.Get("tilewidth").AsInt();
        int tileset_tile_size_y = tileset_json_dictionary.Get("tileheight").AsInt();
        TilesetData local_tilset_data = TilesetData{
            name,
            columns,
            first_gid,
            (float)image_width,
            (float)image_height,
            (float)tileset_tile_size_x,
            (float)tileset_tile_size_y,
            tile_count
        };

        if(name == "actors"){

            Console::Out(image_path+"/"+image);

            Console::Out(
                "[!] Tileset data:\n",
                "name:", name, "\n",
                "columns:",columns,"\n",
                "image width:",image_width,"\n",
                "image height:",image_height,"\n",
                "tile width:",tileset_tile_size_x,"\n",
                "tile height:",tileset_tile_size_y,"\n",
                "tile count:",tile_count
            );
        }
        
        tilemap.tileset_data.push_back(
            local_tilset_data
        );
    }

    for(auto&& layer : level_json.Get("layers").AsArray().Iterable()){
        auto layer_dictionary = layer->AsDictionary();

        if(layer_dictionary.Get("type").AsString() == "imagelayer" && (layer_dictionary.Get("class").AsString() == "" || layer_dictionary.Get("class").AsString() == "BackgroundImage")){
            std::string background_path = layer_dictionary.Get("image").AsString();

            Console::Out("Found BackgroundImage: ",background_path);

            if(background_path == "") continue;

            float parallax_x = 0.0f;
            float parallax_y = 0.0f;
            
            
            if(layer_dictionary.KeyExists("properties")){
                JsonArray& property_json = layer_dictionary.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                        
                    if(_json->AsDictionary().Get("name").AsString() == "parallax_x"){
                        parallax_x = _json->AsDictionary().Get("value").AsFloat();
                    }
                    if(_json->AsDictionary().Get("name").AsString() == "parallax_y"){
                        parallax_y = _json->AsDictionary().Get("value").AsFloat();
                    }
                }
            }

            AssetManager::Get().LoadDecal(relative_path+"/"+background_path, background_path);
            olc::vf2d image_size = AssetManager::Get().GetDecal(background_path)->sprite->Size();

            NewActor(std::make_unique<BackgroundImage>(
                background_path,
                olc::vf2d(0.0f,0.0f),
                olc::vf2d(0.0f,0.0f),
                image_size,
                olc::vf2d(1.0f, 1.0f),
                0.0f, 1, olc::vf2d(parallax_x, parallax_y), 0));
        }

        if(layer_dictionary.Get("type").AsString() == "imagelayer" && layer_dictionary.Get("class").AsString() == "SpriteReference"){
            std::string background_path = layer_dictionary.Get("image").AsString();
            bool visible = layer_dictionary.Get("visible").AsInt();

            Console::Out("Found SpriteReference: ",background_path);

            if(background_path == "") continue;

            asset_manager.LoadDecal(relative_path+"/"+background_path, background_path);
            olc::vf2d image_size = asset_manager.GetDecal(background_path)->sprite->Size();

            level_decals[layer_dictionary.Get("name").AsString()] = asset_manager.GetDecal(background_path);

            auto level_sprite_reference = NewActor<LevelSpriteReference>(
                background_path,
                olc::vf2d(0.0f,0.0f),
                olc::vf2d(0.0f,0.0f),
                image_size,
                olc::vf2d(1.0f, 1.0f),
                0.0f);
            
            level_sprite_reference->visible = visible;
        }

        if(layer_dictionary.Get("type").AsString() == "tilelayer" && layer_dictionary.Get("name").AsString() == "tilemap_collision"){
            std::vector<int> data;
            auto layer_data = layer_dictionary.Get("data").AsArray();

            for(auto&& tile_id : layer_data.Iterable()){
                int id = tile_id->AsInt();
                data.push_back(id);
            }

            Console::Out("Found collision layer");

            tilemap.tilemap_collision_data = data;
        }

        if(layer_dictionary.Get("type").AsString() == "tilelayer" && layer_dictionary.Get("name").AsString() != "tilemap_collision"){
            /*std::vector<int> data;
            auto layer_data = layer.GetObject("data");

            Json::ArrayForEach(
                Json(),layer_data,[&](Json _json, Json _layer_data){
                    int id = _json.GetAsInt();
                    data.push_back(id);
                }
            );

            if(!found_actor_layer) tilemap.layer_data_sets.push_back(data);
            else tilemap.foreground_layer_data_sets.push_back(data);*/

            auto u_tile_map = TileMap::Load(layer_dictionary.AsDictionary());
            if(!found_actor_layer){
                u_tile_map->z_index = -1;
            }
            else{
                u_tile_map->z_index = 2;
            }

            NewActor(std::move(u_tile_map));
        }
        /*if(layer.GetAsString("type") == "imagelayer"){
            float parallax_x = 0.0f;
            float parallax_y = 0.0f;
            std::string image = layer.GetAsString("image");
            std::string image_name = layer.GetAsString("name");
            AssetManager::Get().LoadDecal(relative_path+"/"+image, image_name);
            for(auto&& property_json : layer.GetAsArray("properties")){
                std::string name = property_json.GetAsString("name");
                if(name == "parallax-x") parallax_x = property_json.GetAsDouble("value");
                if(name == "parallax-y") parallax_y = property_json.GetAsDouble("value");
            }
            olc::vf2d decal_size = AssetManager::Get().GetDecal(image_name)->sprite->Size();
            background_system.backgrounds.push_back(BackgroundData{image_name, {parallax_x, parallax_y}, Rectangle({0.0f, 0.0f}, decal_size)});
            Console::Out("parallax:" ,parallax_x, parallax_y);
        }*/
        if(layer_dictionary.Get("type").AsString() == "objectgroup"){
            if(layer_dictionary.Get("name").AsString() == "buttons"){

                for(auto&& _json_a : layer_dictionary.Get("objects").AsArray().Iterable()){
                    std::string on_click = "";
                    std::string on_create = "";
                    float x;
                    float y;
                    float width;
                    float height;
                    std::string text = "";

                    JsonDictionary& property_json = _json_a->AsDictionary();
                    for(auto&& _json_b : property_json.AsArray().Iterable()){
                        //if(!property_json.IsNull()){
                            /*if(_json_b.Get("name").AsString() == "on_click"){
                                on_click = _json_b.GetAsString("value");
                            }
                            if(_json_b.GetAsString("name") == "on_create"){
                                on_create = _json_b.GetAsString("value");
                            }
                            if(_json_b.GetAsString("name") == "text"){
                                text = _json_b.GetAsString("value");
                            }*/
                        //}
                    }
                    
                    x = _json_a->AsDictionary().Get("x").AsInt();
                    y = _json_a->AsDictionary().Get("y").AsInt();
                    width = _json_a->AsDictionary().Get("width").AsInt();
                    height = _json_a->AsDictionary().Get("height").AsInt();
                }
            }
            if(layer_dictionary.Get("name").AsString() == "collision_polygons"){
                Console::Out("found collision polygon layer");
                //collision_system -> alternative_collision_system
                collision_system.Load(layer_dictionary);
            }
            if(layer_dictionary.Get("name").AsString() == "actors"){
                Console::Out("found actor layer");
                found_actor_layer = true;
                LoadActors(layer_dictionary);
            }
            if(layer_dictionary.Get("name").AsString() == "triggers"){
                JsonArray& objects = layer_dictionary.Get("objects").AsArray();
                for(auto&& _object : objects.Iterable()){
                    std::string l_path = "";
                    std::string spawn_point_name = "";
                    
                    if(_object->AsDictionary().KeyExists("properties")){
                        JsonArray& property_json = _object->AsDictionary().Get("properties").AsArray();
                        for(auto&& _json_b : property_json.Iterable()){
                            
                            if(_json_b->AsDictionary().Get("name").AsString() == "path"){
                                l_path = _json_b->AsDictionary().Get("value").AsString();
                            }
                            if(_json_b->AsDictionary().Get("name").AsString() == "spawn_point"){
                                spawn_point_name = _json_b->AsDictionary().Get("value").AsString();
                            }
                            
                        }
                    }

                    float x = (float)_object->AsDictionary().Get("x").AsInt();
                    float y = (float)_object->AsDictionary().Get("y").AsInt();
                    float width = (float)_object->AsDictionary().Get("width").AsInt();
                    float height = (float)_object->AsDictionary().Get("height").AsInt();
                    int tiled_object_id = _object->AsDictionary().Get("id").AsInt();
                    
                    auto u_trigger = std::make_unique<LevelTrigger>(
                        ufo::Rectangle(olc::vf2d(x,y),olc::vf2d(width, height)), l_path, tiled_object_id, spawn_point_name
                    );

                    trigger_handles.push_back(u_trigger.get());
                    level_trigger_handles.push_back(u_trigger.get());

                    NewActor(std::move(u_trigger));
                }
            }
            if(layer_dictionary.Get("name").AsString() == "spawn_points"){
                JsonArray& objects = layer_dictionary.Get("objects").AsArray();
                for(auto&& _object : objects.Iterable()){
                    NewActor(SpawnPoint::Load(_object->AsDictionary()));
                }

                for(const auto& sp : spawn_point_handles){
                    
                    if(sp->name == spawn_point_name){
                        spawn_point = sp->local_position;
                        spawn_mode = SpawnMode::SPAWN_POINT;
                    }
                    
                }
            }
        }

        OnLoadLayer(layer_dictionary);
    }
    
    NewActor(std::make_unique<Shape<ufo::Rectangle>>(ufo::Rectangle(olc::vf2d(0.0f, 0.0f), level_size), olc::vf2d(0.0f, 0.0f))); //This just shows the size of the area.

    olc::vf2d window_size = Engine::Get().pixel_game_engine.GetWindowSize();

    if(Engine::Get().edit_mode) InitializeEditMode();
    Console::Out("[!]", "[Scene::Load()]", "Scene with path:", "'", path , "'", "was loaded.", "It took", benchmarker.Stop(), "nanoseconds to load the scene.");

    if(spawn_mode == SpawnMode::SAVEFILE) OnSaveFileLoad(&(Engine::Get().GetActiveProfile()->save_file));

    OnLoad(level_json);

}

void Level::OnLoad(JsonDictionary& _level_json){
    
}

void Level::OnLoadLayer(JsonDictionary& _layer_json){

}

void Level::LoadActors(JsonDictionary& _json){
    JsonArray objects = _json.Get("objects").AsArray();
    
    for(auto&& _object : objects.Iterable()){
        auto frogatto_actors = tilemap.GetTilesetData("actors");

        Console::Out("Is object null?", _object->IsNull());

        int type_id = _object->AsDictionary().Get("gid").AsInt() - frogatto_actors.tileset_start_id+1;
        switch(type_id){
            case 2:
                //NewActor(TestCollisionBody::Load(_object));
                break;
        }
        OnLoadActors(_object->AsDictionary());
    }
}

void Level::OnLoadActors(JsonDictionary& _actor_json){}
void Level::OnResourceLoad(){}

void Level::InitializeEditMode(){
    NewActor(std::make_unique<ButtonViewCameras>(
        Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x-215, 5, 210, 32));

    NewActor(std::make_unique<ButtonViewSpriteReferences>(
        Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x-215, 400, 210, 32));
    NewActor(std::make_unique<ButtonViewActors>(
        Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x-215, 200, 210, 32));
}

Camera* Level::GetActiveCamera(){
    return camera_handles.back();
}

void Level::ActiveCameraLookAround(bool _condition_move, bool _condition_zoom){
    
    if(_condition_move){
        GetActiveCamera()->local_position-=Mouse::GetDeltaPosition()/GetActiveCamera()->scale;
    }
    GetActiveCamera()->original_local_position = GetActiveCamera()->local_position;
    if(!_condition_zoom) return;
    if(Mouse::GetScrollDirection() > 0) GetActiveCamera()->scale *= 1.05f;
    if(Mouse::GetScrollDirection() < 0) GetActiveCamera()->scale *= (1/1.05f);

}

void Level::OnCameraManipulation(){
    ActiveCameraLookAround(Mouse::GetMiddleButton().is_held, true);
}

void Level::Update(){
    for(auto&& actor : new_actor_queue){
        
        Actor* actor_ptr = actor.get();
        actors.push_back(std::move(actor));
        actor_ptr->UpdateGlobalPosition(olc::vf2d(0.0f, 0.0f));
        actor_ptr->OnStart(this);
    }

    for(auto&& actor : actors){
        actor->AddQueuedChildren();
    }

    new_actor_queue.clear();

    OnCameraManipulation();

    for(const auto& camera : camera_handles){
        camera->EarlyUpdate();
    }

    if(!paused){
        for(const auto& actor : actors){
            actor->Update(olc::vf2d(0.0f, 0.0f));
        }
    }
    else{
        for(const auto& actor : actors){
            actor->OnPaused();
        }
    }

    for(const auto& collision_body : collision_body_handles){
        collision_body->HandleUpdate();
    }

    for(const auto& actor : actors){
        actor->UpdateGlobalPosition(olc::vf2d(0.0f, 0.0f));
    }

    OnUpdate();

    for(auto&& actor : new_actor_queue){
        
        Actor* actor_ptr = actor.get();
        actors.push_back(std::move(actor));
        actor_ptr->UpdateGlobalPosition(olc::vf2d(0.0f, 0.0f));
        actor_ptr->OnStart(this);
    }

    for(auto&& actor : actors){
        actor->AddQueuedChildren();
        actor->UpdateGlobalPosition(olc::vf2d(0.0f, 0.0f));
    }

    for(const auto& camera : camera_handles){
        camera->HandleUpdate();
    }

    /*for(const auto& camera : camera_handles){
        camera->ClampLocalPosition();
    }*/

    new_actor_queue.clear();

    PurgeQueuedForKill();

    if(should_resort_after_z_index){
        std::sort(actors.begin(), actors.end(), actor_sorting_method);
        should_resort_after_z_index = false;
        Console::Out("[!] [Level::Update] Resorted after z_index");
    }

    //To do: Make a call here that readds all contents of new_actor_queue to queued_for_purge
}

void Level::OnUpdate(){
    for(const auto& pb : player_body_handles){
        for(const auto& trig : trigger_handles){
            if(ufoMaths::RectangleVsPoint(trig->shape,pb->position)){
                trig->OnHit(this);
            }
        }
    }
}

void Level::OnPaused(){
    for(const auto& actor : actors){
        actor->OnPaused();
    }
}

void Level::Draw(){
    //return;

    for(const auto& background : background_image_handles){
        background->OnBackgroundDraw(GetActiveCamera());
    }

    OnBackgroundElementDraw(GetActiveCamera());

    if(tilemap_exists) tilemap.Draw(GetActiveCamera());
    for(const auto& actor : actors){
        actor->Draw(GetActiveCamera());
    }
    if(tilemap_exists) tilemap.DrawForeGround(GetActiveCamera());

    collision_system.Draw(GetActiveCamera());
    for(const auto& actor : actors){
        actor->WidgetDraw();
    }

    //auto d = olc::Decal(&pixel_effect_sprite);
    //d.Update();

    //pixel_effect_decal->Update();
    //Engine::Get().pixel_game_engine.DrawDecal(olc::vf2d(0.0f, 0.0f),&d);
    //d.Update();

    //pixel_effect_sprite = empty_sprite;
    //pixel_effect_decal = olc::Decal(pixel_effect_sprite);

}

void Level::OnBackgroundElementDraw(Camera* _camera){

}

void Level::QueueForPurge(int _id){
    queued_for_purge.push_back(_id);
}

void Level::PurgeQueuedForKill(){
    if(queued_for_purge.size() == 0) return; //Should here be no queued actors

    std::vector<int> additional_ids_to_purge; //This should be removed man
    
    //This looks for all dead children and adds them to additional_queued_for_purge
    for(int id : queued_for_purge){
        for(int i = actors.size() - 1; i >= 0; i--){
            actors[i]->SearchForDeadActors(id);
        }
    }
    for(int i : additional_queued_for_purge) queued_for_purge.push_back(i);
    additional_queued_for_purge.clear();

    PurgeHandles(camera_handles);
    PurgeHandles(widget_handles);
    PurgeHandles(sprite_handles);
    PurgeHandles(collision_body_handles);
    PurgeHandles(player_body_handles);
    PurgeHandles(background_image_handles);
    
    OnHandlePurge();

    for(int i = actors.size() - 1; i >= 0; i--){
        if(!actors[i]->dead) actors[i]->PurgeDeadActors();
        else actors.erase(actors.begin()+i);
    }
    queued_for_purge.clear();
    
}

void Level::OnHandlePurge(){
    
}

void Level::OnTransition(Level* _level){

    for(auto&& sp : _level->spawn_point_handles){
        for(const auto& hit_trigger : hit_level_triggers){
            Console::Out("hello world");

            //if spawn-point name matches the one specified in the trigger player last collided with
            if(sp->name == hit_trigger->spawn_point_name){

                //If you have a metroid vania game where you want the player to retain the same attribtues
                //through the transition you can std::move it instead of copying each attribute.
                //It's safe if you make sure to refetch all resources by using OnLevelEnter instead of constructor
                if(retain_player_properties){
                    for(const auto& next_level_player_body : _level->player_body_handles){
                        next_level_player_body->QueueForPurge();
                    }
                    for(const auto& pb : player_body_handles){
                        pb->local_position = sp->local_position;
                        for(auto&& actor : actors){
                            if(actor->id == pb->id){
                                _level->NewActor(std::move(actor));
                            }
                        }
                    }
                }
                else{
                    for(const auto& pb : _level->player_body_handles){
                        pb->local_position = sp->local_position;
                    }
                }
            }
        }
    }
    
}

void Level::OnSaveFileLoad(JsonVariant* _json){

}

void Level::OnSave(JsonVariant* _current_savefile){
    
}

void Level::Save(JsonVariant* _current_savefile){

    Console::Out(savable_actor_handles.size());

    for(const auto& savable : savable_actor_handles){
        savable->OnSave(_current_savefile);
    }
    OnSave(_current_savefile);
}

void Level::OnExit(){
    Console::Out("Exited level");
}

void Level::DebugDraw(){
    for(const auto& actor : actors){
        actor->DebugDraw(GetActiveCamera());
    }
    tilemap.DrawCollisionTiles(GetActiveCamera());

    if(!Engine::Get().edit_mode) return;

    olc::vf2d window_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();
    DrawingSystem::DrawFilled(ufo::Rectangle(olc::vf2d(0.0f, window_size.y-18.0f), olc::vf2d(window_size.x,18.0f)),olc::VERY_DARK_CYAN);
    olc::vf2d current_mouse_position = GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
    Engine::Get().pixel_game_engine.DrawStringDecal(
        olc::vf2d(2.0f,window_size.y-17.0f),
        "Mouse position: "+
        std::to_string((int)current_mouse_position.x)+ "," +
        std::to_string((int)current_mouse_position.y), olc::WHITE, {1.0f, 2.0f});

}

Level::~Level(){
    Console::Out("[!] Level succcessfully destructed");
}