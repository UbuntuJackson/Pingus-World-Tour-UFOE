#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <functional>
#include "../shapes/rectangle.h"
#include "../shapes/circle.h"
#include "../camera/camera.h"
#include "../camera/transformations.h"
#include "../ufo_sprite/ufo_sprite.h"
#include "../json/json.h"
#include "../ufo_constants/ufo_constants.h"
#include "../ufo_vector_utils/is_in_vector.h"
#include "../drawing_system/drawing_system.h"
#include "../garbage_collection/ownership_id_node.h"
#include "../creation_data/creation_data.h"
#include "../default_objects/coin.h"
#include "../player_body/player_body.h"
#include "../shapes/ray2.h"
#include "../graphics_provider/graphics_provider.h"
#include "../ufo_object/ufo_object.h"
#include "../tilemap/tilemap.h"
#include "../background/background_system.h"
#include "../menu/button.h"
#include "../menu/menu_system.h"
#include "../animation/animation_bundle.h"
#include "../player_body/top_down_body.h"
#include "../shapes/colour_shape.h"

class SceneSystem;

class Scene{
    olc::vf2d world_size;
    int global_camera_id;
public:
    SceneSystem* scene_system;

    Scene();
    ~Scene();
    void OnStart();

    int scene_id_count;
    int GenerateSceneID();
    std::unordered_map<int, std::string> immortal_object_ids;

    Tilemap tilemap;

    std::string path;
    void Load(std::string _path);
    void OnLoad(std::string _path);    

    typedef std::function<bool()> Event; //Event is a function that the engine will delete if the event returns true
    typedef std::function<void()> Task; //Task is a function that runs during the entire runtime and should not be detached.
    typedef std::function<void(Json _json)> LoadEvent;
    std::vector<Event> start_events;

    std::vector<Event> events;
    std::vector<Event> garbage_collection_events;
    std::vector<Task> tasks;

    std::unique_ptr<OwnerShipID_Node> ownership_id_node_tree_root;
    //                          //the node would be like the owner, the _actor_id with an empty unordered_map would be like a child with no contents
    OwnerShipID_Node* AddDependancy(OwnerShipID_Node* _node ,int _actor_id);
    OwnerShipID_Node* GetDependancyTreeRoot();
    int ownership_node_count;

    std::unordered_map<int, std::unique_ptr<Camera>> cameras; //owner, camera
    std::vector<int> camera_hierarchy;
    std::unordered_map<int, std::pair<std::unique_ptr<ufoSprite>, bool>> ufo_sprites;
    std::unordered_map<int, std::unique_ptr<Coin>> coins;
    std::unordered_map<int, std::unique_ptr<PlayerBody>> player_bodies;
    std::unordered_map<int, std::unique_ptr<TopDownBody>> top_down_bodies;

    std::unordered_map<int, std::unique_ptr<Ray2>> line_segments;
    std::unordered_map<int, std::unique_ptr<Ray2>> user_rays;
    std::unordered_map<int, std::unique_ptr<ufoObjectBase>> ufo_objects; //rename this to something
    std::unordered_map<int, std::unique_ptr<Button>> buttons;
    std::unordered_map<int, std::unique_ptr<AnimationBundleReference>> animation_bundle_references;
    
    bool show_all_shapes = false;
    std::unordered_map<int, std::unique_ptr<ColourShapeBase>> colour_shapes;
    std::unordered_map<int, std::unique_ptr<Rectangle>> rectangles;
    std::unordered_map<int, std::unique_ptr<Circle>> circles;

    //Tilemap tilemap;
    DrawingSystem drawing_system;
    BackgroundSystem background_system;
    MenuSystem menu_system;
    //void UpdateDrawingSystem();
    //void UpdatePlayerBodies();

    olc::vf2d GetWorldSize();
    Camera* GetGlobalCamera();

    CreationData<Camera> NewCamera(
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
    );

    Camera* GetActiveCamera();

    olc::vf2d GetMousePositionInWorld();

private:
    std::vector<int> dead;

public:
    void Kill(int _id);

    template <typename tType>
    std::vector<tType*> GetActors(int _id, std::unordered_map<int, tType> &_map){
        std::vector<tType*> vec;
        for(auto&& [k, v] : _map){
            if(k == _id) vec.push_back(v.get());
        }
        return vec;
    }

    template <typename tMap>
    void UpdateActors(tMap& _map){
        for(auto&& [k, v] : _map){

            if((std::find(dead.begin(), dead.end(), k) == dead.end())){
                _map.at(k)->Update();
                _map.at(k)->OnUpdate();
            }
        }
    }

    template <typename tMap>
    void GarbagecollectActors(tMap& _map, std::string _typ = "<unknown>"){
        int garbage_actors_found = 0;

        for(auto&& [k, v] : _map){
            if((std::find(dead.begin(), dead.end(), k) != dead.end())){
                garbage_actors_found++;
                garbage_collection_events.push_back(
                    [&](){
                        _map.erase(k); return true;
                    
                    }
                );
            }
        }
        if(!garbage_actors_found) return;
        Console::Out("[!]", "[Scene::GarbagecollectActors()              ]", garbage_actors_found, "actor(s) of type:", _typ, "were garbage collected this frame.");
    }

    void ActiveCameraLookAround(bool _condition_move, bool _condition_zoom = true);
    void Update_ufoSprites();
    void UpdateRectangles();
    void UpdatePlayerBodies();
    void UpdateAnimations();
    void UpdateMenuSystem();
    void UpdateShapes();
    void DrawColourShapes();
    void DrawShapes();
    virtual void OnUpdate();
    void UpdateDefaultSceneContents();
    void MarkAsDeadRecursive(OwnerShipID_Node *_ownership_id_node, bool _dead_found = false);
    void GarbageCollectDefaultSceneContents();
    virtual void Update();
    int DrawNodeTree(OwnerShipID_Node* _node, int _dent, int _height);

    template<typename tType>
    std::string
    GetActorMapAsString(std::string _actor_type, tType &_m){ //maybe return string to add to strings to draw
        std::string s = "";
        for(auto&& [k,v] : _m){
            s += ("["+ std::to_string(k) + "]");
        }
        return _actor_type + s;
    }

    template<typename tType>
    void
    UpdateActorInspectorMenu(int _row_number, const std::string& _type_name ,std::unordered_map<int, tType> *_m){

        int entry_incrementor = 0;
        int entry_height = 48;
        std::string actor_map_as_string = GetActorMapAsString(_type_name, *_m);
        auto button_lengths = GetInspectorButtons(_m);

        for(auto&& [k, v] : *_m){
            int length = button_lengths.at(k);
            Rectangle entry_rect = Rectangle(olc::vf2d(_type_name.size()*16.0f+entry_incrementor*length*16.0f, _row_number*entry_height), olc::vf2d(16.0f*length, 16.0f));
            entry_incrementor++;
            if(ufoMaths::RectangleVsPoint(entry_rect, Mouse::GetPosition()) && Mouse::GetLeftButton().is_pressed){
                Kill(k);
            }
            GraphicsProvider::Get().FillRect(entry_rect.position, entry_rect.size, olc::BLACK);
        }

        GraphicsProvider::Get().DrawStringDecal({0.0f, (float)_row_number*entry_height}, actor_map_as_string, olc::WHITE, {2.0f, 2.0f});

    }

    template<typename tType>
    std::unordered_map<int, int> //id, width
    GetInspectorButtons(std::unordered_map<int, tType> *_m){
        std::unordered_map<int, int> o;
        for(const auto& [k,v] : *_m){
            int digits = 0;
            int number = k;
            if(number < 0) digits = 1;
            while(number){
                number/=10;
                digits++;
            }
            //Console::Out(digits);
            o.emplace(k, digits + 2);
        }
        return o;
    }

    void UpdateEditor();
    void UpdateActiveActorMenu();

    void UpdateCameras();
    //Maybe make each "New" function a template function and induce constructor with olc::vf2d* _position instead in case of attachment.
    CreationData<ufoSprite> NewSprite(std::string _key, olc::vf2d _position, olc::vf2d _offset = {0.0f, 0.0f}, float _scale = 1.0f, float _rotation = 0.0f, int _z = 0);
    CreationData<ufoSprite> NewSpriteSheet(std::string _key, olc::vf2d _position, olc::vf2d _frame_size, olc::vf2d _offset = {0.0f, 0.0f}, int _z = 0);

    CreationData<Coin> NewCoin(olc::vf2d _position, int _value);
    CreationData<PlayerBody> NewPlayerBody(olc::vf2d _position);

    template<typename tType>
    int New_ufoObject(tType* _handle, std::function<void(tType*)> _task){
        int id = GenerateSceneID();
        ufo_objects[id] = std::make_unique<ufoObject<tType>>(_handle, _task);
        return id;
    }

    template <typename tType, typename tUnorderedMap, typename ...tArgs>
    CreationData<tType> AddNewActorToMap(tUnorderedMap& _map, tArgs ..._args){
        std::unique_ptr<tType> unique = std::make_unique<tType>(_args...);
        auto pointer = unique.get();
        int id = GenerateSceneID();
        _map[id] = std::move(unique);
        return CreationData<tType>{pointer, id};
    }

    //CreationData<TopDownBody> NewTopDownBody(olc::vf2d _position, olc::vf2d _size);
};

#endif