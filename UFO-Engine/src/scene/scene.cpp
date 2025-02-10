#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <cmath>
#include "scene.h"
#include "../asset_manager/asset_manager.h"
#include "../external/olcPixelGameEngine.h"
#include "../ufo_sprite/ufo_sprite.h"
#include "../console/console.h"
#include "../drawing_system/drawing_system.h"
#include "../asset_manager/asset_manager.h"
#include "../mouse/mouse.h"
#include "../ufo_vector_utils/is_in_vector.h"
#include "../shapes/circle.h"
#include "../ufo_benchmarker/ufo_benchmarker.h"
#include "../ufo_constants/ufo_constants.h"
#include "../shapes/rectangle.h"
#include "../keyboard/single_keyboard.h"
#include "../creation_data/creation_data.h"
#include "../default_objects/coin.h"
#include "../camera/transformations.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_collision/ufo_collision.h"
#include "../json/json.h"
#include "../ufo_vector_utils/range.h"
#include "../ufo_object/ufo_object.h"
#include "../tilemap/tileset_data.h"
#include "../animation/animation_bundle.h"
#include "../animation/animation_manager.h"
#include "../ufo_benchmarker/ufo_benchmarker.h"
#include "../player_body/top_down_body.h"
#include "../ufo_lang/ufo_lang.h"
#include "../scene_system/scene_system.h"
#include "../shapes/new_shape.h"

Scene::Scene(){
    scene_id_count = 0;
    path = "";
    drawing_system.scene = this;
    menu_system.scene = this;
    ownership_id_node_tree_root = std::make_unique<OwnerShipID_Node>();
    ownership_node_count = 0;
}

Scene::~Scene(){
    for(auto&& key : tilemap.keys_to_temporary_assets){
        AssetManager::Get().RemoveAsset(key);
        Console::Out("[!]", "Removed temporary asset:", key);
    }
    for(auto&& bg : background_system.backgrounds){
        AssetManager::Get().RemoveAsset(bg.key);
        Console::Out("[!]", "Removed temporary asset:", bg.key);
    }
}

void
Scene::OnStart(){
    ufoLang::Test(this);
    
    //don't touch
    olc::vf2d WORLD_SIZE = GetWorldSize();
    global_camera_id = NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y).id;
    immortal_object_ids[global_camera_id] = "Global Camera";

    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewCamera(olc::vf2d(0.0f, 0.0f), ufo::NONE_ID, 0.0f, 1600, 0.0f, 800, 0.0f, WORLD_SIZE.x, 0.0f, WORLD_SIZE.y);
    NewRectangle(this, 0.0f, 0.0f, WORLD_SIZE.x, WORLD_SIZE.y);

    //Custom stuff...

    AssetManager::Get().LoadDecal("../res/test/dock.png", "dock");
    AssetManager::Get().LoadDecal("../res/player_body/player_body_default.png", "player_body_default");
    AssetManager::Get().LoadDecal("../res/deathcap.png", "deathcap");
    /*
    {
    auto duck_sprite = NewSprite("dock", olc::vf2d(0.0f, 0.0f));
    auto node = NewOwnershipID_Node(ownership_id_node_tree_root.get(), duck_sprite.id);
    {
    auto rectangle = NewRectangle(40.0f, 40.0f, 800.0f, 800.0f);
    NewOwnershipID_Node(node, rectangle.first.id);
    }
    {
    auto rectangle = NewRectangle(40.0f, 40.0f, 8.0f, 8.0f);
    NewOwnershipID_Node(node, rectangle.first.id);
    }
    {
        auto rectangle = NewRectangle(1000.0f, 40.0f, 500.0f, 800.0f);
        auto rect_node = NewOwnershipID_Node(node ,rectangle.first.id);
        {
        auto rectangle2 = NewRectangle(1000.0f, 40.0f, 500.0f, 800.0f);
        NewOwnershipID_Node(rect_node, rectangle2.first.id);
        }
        {
        auto rectangle2 = NewRectangle(1000.0f, 40.0f, 500.0f, 800.0f);
        NewOwnershipID_Node(rect_node, rectangle2.first.id);
        }
        {
        auto rectangle2 = NewRectangle(1000.0f, 40.0f, 500.0f, 800.0f);
        NewOwnershipID_Node(rect_node, rectangle2.first.id);
        }
    }
    {
    auto rectangle = NewRectangle(40.0f, 40.0f, 1000.0f, 200.0f);
    NewOwnershipID_Node(node, rectangle.first.id);
    }
    }*/
    //NewPlayerBody({300.0f, 300.0f});

    NewUserRay2(this, olc::vf2d(200.0f, 200.0f), olc::vf2d(1000.0f, 500.0f));
    NewLineSegment2(this, olc::vf2d(500.0f, 200.0f), olc::vf2d(20.0f, 500.0f));

    AnimationManager::Get().Load("../res/tuxtale/gfx/Tux/tuxtale-animation/", "tuxtale-animation.json","walk" , "walk-horizontal");

    animation_bundle_references[GenerateSceneID()] = std::make_unique<AnimationBundleReference>("walk", olc::vf2d(0.0f, 0.0f));

    NewCircle(this, 450.0f, 450.0f, 300.0f, olc::VERY_DARK_BLUE, true);
    ::NewRectangle(this, 450.0f, 450.0f, 300.0f, 500.0f, olc::VERY_DARK_BLUE, true);
}

int Scene::GenerateSceneID(){
    return scene_id_count++;
}

void Scene::Load(std::string _path){
    ufoBenchMarker benchmarker;

    Json scene_json = Json::Read(_path);

    std::string relative_path = "";
    for(auto&& property_json : scene_json.GetAsArray("properties")){
        std::string name = property_json.GetAsString("name");
        if(name == "relative path") relative_path = property_json.GetAsString("value");
        Console::Out("[!!!]", "Using relative path:", relative_path, ".", "Should be resolves asap.");
    }

    int tile_size_x = scene_json.GetAsInt("tilewidth");
    int tile_size_y = scene_json.GetAsInt("tileheight");
    int map_size_x = scene_json.GetAsInt("width");
    int map_size_y = scene_json.GetAsInt("height");
    world_size = {(float)tile_size_x*map_size_x,(float)tile_size_y*map_size_y};
    tilemap.tilemap_width = map_size_x;
    for(auto&& layer : scene_json.GetAsArray("layers")){
        if(layer.GetAsString("type") == "tilelayer"){
            std::vector<int> data;
            auto layer_data = layer.GetObject("data");
            /*for(int i = 0; i < layer_data.GetArrayLength(); i++){
                int id = layer_data.GetElement(i).GetAsInt();
                tilemap.tiles.push_back(id);
            }*/

            Json::ArrayForEach(
                Json(),layer_data,[&](Json _json, Json _layer_data){
                    int id = _json.GetAsInt();
                    data.push_back(id);
                }
            );
            tilemap.layer_data_sets.push_back(data);
        }
        if(layer.GetAsString("type") == "imagelayer"){
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
        }
        if(layer.GetAsString("type") == "objectgroup"){
            if(layer.GetAsString("name") == "buttons"){

                Json::ArrayForEach( Json(), layer.GetObject("objects"), [&](Json _json_a, Json _data_a){
                    std::string on_click = "";
                    std::string on_create = "";
                    float x;
                    float y;
                    float width;
                    float height;
                    std::string text = "";

                    Json property_json = _json_a.GetObject("properties");
                    Json::ArrayForEach(Json(), property_json, [&](Json _json_b, Json _data_b){
                        if(property_json.member){
                            if(_json_b.GetAsString("name") == "on_click"){
                                on_click = _json_b.GetAsString("value");
                                Console::Out(on_click);
                            }
                            if(_json_b.GetAsString("name") == "on_create"){
                                on_create = _json_b.GetAsString("value");
                                Console::Out(on_create);
                            }
                            if(_json_b.GetAsString("name") == "text"){
                                text = _json_b.GetAsString("value");
                            }
                        }
                    }
                    );
                    
                    x = _json_a.GetAsInt("x");
                    y = _json_a.GetAsInt("y");
                    width = _json_a.GetAsInt("width");
                    height = _json_a.GetAsInt("height");

                    buttons[GenerateSceneID()] = std::make_unique<Button>(x,y,width,height,on_create,on_click,text);
                }
                );
            }
        }
    }

    Console::Out("[!]", "Currently loading tilesets");

    for(auto&& tileset_json : scene_json.GetAsArray("tilesets")){
        std::string image = tileset_json.GetAsString("image");
        std::string name = tileset_json.GetAsString("name");
        AssetManager::Get().LoadDecal(relative_path+"/"+image, name);
        tilemap.keys_to_temporary_assets.push_back(name);

        int columns = tileset_json.GetAsInt("columns");
        int first_gid = tileset_json.GetAsInt("firstgid");
        int image_height = tileset_json.GetAsInt("imageheight");
        int image_width = tileset_json.GetAsInt("imagewidth");
        int tile_count = tileset_json.GetAsInt("tilecount");
        int tileset_tile_size_x = tileset_json.GetAsInt("tilewidth");
        int tileset_tile_size_y = tileset_json.GetAsInt("tileheight");
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
        
        tilemap.tileset_data.push_back(
            local_tilset_data
        );
    }

    Console::Out("[!]", "[Scene::Load()]", "Scene with path:", "'", _path , "'", "was loaded.", "It took", benchmarker.Stop(), "nanoseconds to load the scene.");
}

void Scene::OnLoad(std::string _path){

}

OwnerShipID_Node* Scene::AddDependancy(OwnerShipID_Node* _node ,int _actor_id){
    ownership_node_count++;
    _node->property[_actor_id] = std::make_unique<OwnerShipID_Node>();
    return _node->property.at(_actor_id).get();
}

OwnerShipID_Node* Scene::GetDependancyTreeRoot(){
    return ownership_id_node_tree_root.get();
}

olc::vf2d Scene::GetWorldSize(){
    return world_size;
}

Camera* Scene::GetGlobalCamera(){
    return cameras.at(global_camera_id).get();
}

CreationData<Camera>
Scene::NewCamera(
    olc::vf2d _position,
    int _target_id,
    int _x_view_0,
    int _x_view_1,
    int _y_view_0,
    int _y_view_1,
    int _world_x0,
    int _world_x1,
    int _world_y0,
    int _world_y1
){
    int id = GenerateSceneID();
    cameras[id] = std::move(std::make_unique<Camera>(
        _position,
        _target_id,
        _x_view_0,
        _x_view_1,
        _y_view_0,
        _y_view_1,
        _world_x0,
        _world_x1,
        _world_y0,
        _world_y1
    ));
    camera_hierarchy.push_back(id);
    return CreationData<Camera>{cameras.at(id).get(), id};
}

Camera* Scene::GetActiveCamera(){
    return cameras.at(camera_hierarchy.back()).get();
}

olc::vf2d
Scene::GetMousePositionInWorld(){
    return GetActiveCamera()->Transform(Mouse::GetPosition());
}

void
Scene::Kill(int _id){
    if(!immortal_object_ids.count(_id)) dead.push_back(_id);
    else Console::Out("[!]", "[Scene::Kill()]", immortal_object_ids.at(_id), "is an immortal object and cannot be killed.");
}

void
Scene::Update_ufoSprites(){
    for(auto&& [id, sprite] : ufo_sprites){
        sprite.first->position = *(sprite.first->parent_position);
        drawing_system.ufo_sprites.push_back(std::make_pair<ufoSprite*, bool>(sprite.first.get(), bool(sprite.second)));
    }
}

void Scene::UpdateCameras(){
    GetGlobalCamera()->position = GetActiveCamera()->position;
}

void
Scene::UpdatePlayerBodies(){
    
}

void
Scene::UpdateAnimations(){
    for(auto&& [id, animation_bundle_reference] : animation_bundle_references){
        animation_bundle_reference->Update(this);
    }
}

void
Scene::UpdateMenuSystem(){
    menu_system.Update(&buttons, GetActiveCamera());
    menu_system.Draw(&buttons, GetActiveCamera());
}

void
Scene::UpdateShapes(){
    /*for(auto&& [j, user_ray] : user_rays){
        std::vector<Ray2> hit_segments;
        for(auto&& [i, line_segment] : line_segments){
            auto hit_check = ufoMaths::RayVsRay(user_ray->ray, line_segment->ray);
            if(hit_check.is_hit){
                user_ray->collision_data = hit_check;
                hit_segments.push_back(line_segment->ray);
            }
        }
        for(auto&& segment : hit_segments){
            user_ray->ray.p1 += ufoMaths::GetDeflectionVector(user_ray->ray.p0, user_ray->ray.Size(), segment);
        }
    }*/
}

void
Scene::DrawColourShapes(){
    for(const auto& [key,item] : colour_shapes){
        if(item->permanently_visible || show_all_shapes) item->Update(GetActiveCamera());
    }
}

void
Scene::DrawShapes(){
    /*for(auto&& [k, v] : line_segments){
        auto transformed_ray = TransformRay2(&(v->ray), GetActiveCamera());
        GraphicsProvider::Get().DrawLineDecal(transformed_ray.Start(), transformed_ray.End());
        auto normal = Ray2(v->ray.Start() + v->ray.Size()/2.0f, v->ray.Start() + v->ray.Size()/2.0f + olc::vf2d(v->ray.Size().y, -v->ray.Size().x).norm()*10.0f);
        auto transformed_normal = TransformRay2(&(normal), GetActiveCamera());
        GraphicsProvider::Get().DrawLineDecal(transformed_normal.Start(), transformed_normal.End());
    }
    for(auto&& [k, v] : user_rays){
        auto transformed_ray = TransformRay2(&(v->ray), GetActiveCamera());
        if(v->collision_data.is_hit) GraphicsProvider::Get().DrawLineDecal(transformed_ray.Start(), transformed_ray.End(), olc::RED);
        else GraphicsProvider::Get().DrawLineDecal(transformed_ray.Start(), transformed_ray.End(), olc::BLUE);
    }*/
}

/*
template<typename tMap>
void Scene::ForEachItemMap(tMap& _map){
    for(auto&& [k, v] : _map) OnEachItemInMap(k, v);
}

void Scene::OnUpdateEachItemInMap(int _key, int _type){

}
*/

void Scene::ActiveCameraLookAround(bool _condition_move, bool _condition_zoom){
    if(_condition_move) GetActiveCamera()->position-=Mouse::GetDeltaPosition()/GetActiveCamera()->scale;
    if(!_condition_zoom) return;
    if(Mouse::GetScrollDirection() > 0) GetActiveCamera()->scale *= 1.05f;
    if(Mouse::GetScrollDirection() < 0) GetActiveCamera()->scale *= (1/1.05f);
}

void
Scene::OnUpdate(){
    if(SingleKeyboard::GetKey(olc::S).is_pressed) show_all_shapes = !show_all_shapes;
    ActiveCameraLookAround(Mouse::GetMiddleButton().is_held, SingleKeyboard::GetKey(olc::SHIFT).is_held);
}

void
Scene::UpdateDefaultSceneContents(){
    UpdateActors(player_bodies);
    UpdateCameras();
    UpdateActors(ufo_objects);
    background_system.Update(GetActiveCamera());
    tilemap.Update(GetActiveCamera());
    Update_ufoSprites();
    UpdateShapes();
    drawing_system.Update();
    DrawColourShapes();
    DrawShapes();
    UpdateAnimations();
    UpdateMenuSystem();
    UpdateActiveActorMenu();
}

void
Scene::MarkAsDeadRecursive(OwnerShipID_Node *_ownership_id_node, bool _dead_found){
    std::vector<int> keys;
    for(auto&& [k,v] : _ownership_id_node->property){        
        if(IsInVector(dead, k) || _dead_found){
            Kill(k);
            ownership_node_count--;
            MarkAsDeadRecursive(v.get(), true);
            keys.push_back(k);
        }
        else{
            MarkAsDeadRecursive(v.get());
        }
    }
    for(auto k : keys){
        _ownership_id_node->property.erase(k);
    }
}

void
Scene::GarbageCollectDefaultSceneContents(){
    ufoBenchMarker b;

    MarkAsDeadRecursive(ownership_id_node_tree_root.get());
    GarbagecollectActors(cameras, "Camera");
    std::vector<int> cameras_to_remove;
    for(int i = camera_hierarchy.size()-1; i > 0; i--){
        if(IsInVector(dead, camera_hierarchy[i])) cameras_to_remove.push_back(i);
    }
    for(int index : cameras_to_remove){
        camera_hierarchy.erase(camera_hierarchy.begin()+index);
    }

    /*Console::Out(dead.size());
    Console::Out(camera_hierarchy.size());
    Console::Out(cameras_to_remove.size());*/
    GarbagecollectActors(ufo_sprites, "ufoSprite");
    GarbagecollectActors(circles, "Circle");
    GarbagecollectActors(rectangles, "Rectangle");
    GarbagecollectActors(player_bodies, "PlayerBody");
    dead.clear();

    for(int event_index = 0; event_index < garbage_collection_events.size(); event_index++){
        garbage_collection_events[event_index]();
    }
    if(garbage_collection_events.size() > 0) Console::Out("[!]", "[Scene::GarbageCollectDefaultSceneContents()]", "Garbage collection took", b.Stop(), "nanoseconds.");
    garbage_collection_events.clear();
}

void
Scene::Update(){
    UpdateDefaultSceneContents();
    OnUpdate();
    
    std::vector<Event> leftover_events;
    for(int event_index = 0; event_index < events.size(); event_index++){
        if(!events[event_index]()) leftover_events.push_back(events[event_index]);
    }

    events.clear();
    for(Event event : leftover_events) events.push_back(event);
    for(Task task : tasks){
        task();
    }
    
    GarbageCollectDefaultSceneContents();
    Mouse::Get().Update();
    DrawNodeTree(ownership_id_node_tree_root.get(), 0, 0);
}

int
Scene::DrawNodeTree(OwnerShipID_Node* _node, int _dent, int _height){
    int local_height = 0;
    for(auto&& [key, node] : _node->property){

        GraphicsProvider::Get().DrawStringDecal({0.0f+_dent*32, (_height+local_height)*32.0f}, std::to_string(key), olc::WHITE, {2.0f, 2.0f});
        local_height++;
        local_height +=DrawNodeTree(node.get(), _dent+1, _height+local_height);
        
    }
    return local_height;
}

void
Scene::UpdateEditor(){
    
    /*if(!selected){
        for(auto&& [k,v] : line_segments){
            if(Distance2(v->shape.p0, GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition())) <= 4.0f){
                
            }
            if(Distance2(v->shape.p1, GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition())) <= 4.0f){
                if(Mouse::GetLeftButton().is_held) ;
            }
        }
    }*/
}

void
Scene::UpdateActiveActorMenu(){
    UpdateActorInspectorMenu(0, "Cameras: ",&cameras);
    UpdateActorInspectorMenu(1, "Rectangles: ",&rectangles);
}

CreationData<ufoSprite>
Scene::NewSprite(std::string _key, olc::vf2d _position, olc::vf2d _offset, float _scale, float _rotation, int _z){
    auto unique = std::make_unique<ufoSprite>(_key, _position, AssetManager::Get().GetDecal(_key)->sprite->Size(), _offset, _scale, _rotation, _z);
    auto pointer = unique.get();
    int id = GenerateSceneID();
    ufo_sprites[id] = std::make_pair<std::unique_ptr<ufoSprite>, bool>(std::move(unique), true);
    return CreationData<ufoSprite>{pointer, id};
}

CreationData<ufoSprite>
Scene::NewSpriteSheet(std::string _key, olc::vf2d _position, olc::vf2d _frame_size, olc::vf2d _offset, int _z){
    auto unique = std::make_unique<ufoSprite>(_key, _position, _frame_size, _offset, _z);
    auto pointer = unique.get();
    int id = GenerateSceneID();
    ufo_sprites[id] = std::make_pair<std::unique_ptr<ufoSprite>, bool>(std::move(unique), true);
    return CreationData<ufoSprite>{pointer, id};
}

CreationData<Coin>
Scene::NewCoin(olc::vf2d _position, int _value){
    std::unique_ptr<Coin> unique = std::make_unique<Coin>(_position, _value);
    auto pointer = unique.get();
    int id = GenerateSceneID();
    coins[id] = std::move(unique);

    //attach sprite
    auto ownership_node = AddDependancy(ownership_id_node_tree_root.get(), id);

    CreationData<ufoSprite> sprite = NewSprite("dock", olc::vf2d(0.0f, 0.0f));
    //sprite.Depends(pointer->position);

    AddDependancy(ownership_node, sprite.id);
    
    return CreationData<Coin>{pointer, id};
}

CreationData<PlayerBody>
Scene::NewPlayerBody(olc::vf2d _position){
    std::unique_ptr<PlayerBody> unique = std::make_unique<PlayerBody>(_position, GetActiveCamera());
    auto pointer = unique.get();
    int id = GenerateSceneID();
    player_bodies[id] = std::move(unique);

    //attach sprite
    auto ownership_node = AddDependancy(GetDependancyTreeRoot(), id);

    CreationData<ufoSprite> sprite = NewSprite("player_body_default", olc::vf2d(0.0f, 0.0f), olc::vf2d(50.0f, 100.0f));
    sprite.ptr->SetParentPosition(&(pointer->position));

    AddDependancy(ownership_node, sprite.id);

    GetActiveCamera()->clamp = true;
    
    return CreationData<PlayerBody>{pointer, id};
}

/*CreationData<TopDownBody> Scene::NewTopDownBody(olc::vf2d _position, olc::vf2d _size){
    auto creation_data = AddNewActorToMap<TopDownBody>(top_down_bodies, _position, GetActiveCamera());
    auto collider_creation_data = NewDynamicRectangle(_position, _size);
    collider_creation_data.ptr->SetParentPosition(&creation_data.ptr->position);
    return creation_data;
}*/