namespace TuxTale{
#include "../scene/scene.h"
#include "../external/olcPixelGameEngine.h"

void SpawnTux(Scene* _scene, olc::vf2d _position){
    std::unique_ptr<PlayerBody> unique = std::make_unique<PlayerBody>(_position, GetActiveCamera());
    auto pointer = unique.get();
    int id = GenerateSceneID();
    player_bodies[id] = std::move(unique);

    //attach sprite
    auto ownership_node = NewOwnershipID_Node(ownership_id_node_tree_root.get(), id);

    CreationData<ufoSprite> sprite = NewSprite("player_body_default", olc::vf2d(0.0f, 0.0f), olc::vf2d(50.0f, 100.0f));
    sprite.ptr->SetParentPosition(&(pointer->position));

    NewOwnershipID_Node(ownership_node, sprite.id);

    GetActiveCamera()->clamp = true;
    
    return CreationData<PlayerBody>{pointer, id};
}

}