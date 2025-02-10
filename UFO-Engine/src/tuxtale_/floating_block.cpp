#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "floating_block.h"
#include "tuxtale_player.h"
#include "tuxtale_level.h"
#include "../tilemap/tile_map.h"

FloatingBlock::FloatingBlock(Vector2f _local_position) : Actor(_local_position){

}

void
FloatingBlock::OnLevelEnter(Level* _level){
    level = dynamic_cast<TuxTaleLevel*>(_level);
    level->floating_block_handles.push_back(this);

    auto u_shadow = level->NewActor<SpriteReference>(
        "save_point_shadow",
        local_position,
        Vector2f(0.0f, 0.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(1.0f, 1.0f),
        0,
        0
    );

    SetZIndex(4);
}

void FloatingBlock::OnUpdate(){
    if(ready) return;
    ready = true;

    std::vector<TileMap*> tilemaps = level->GetActorsOfType<TileMap>();

    float former_z_index = z_index;

    for(const auto& tilemap : tilemaps){
        if(tilemap->z_index < 2) continue;
        
        bool stop_iterating = false;
        for(int yy = -1; yy <= 1; yy++){
            for(int xx = -1; xx <= 1; xx++){
                Vector2f check_location = local_position+Vector2f(1.0f, 1.0f);

                int tile_id = tilemap->GetTileID_AtLevelPosition(check_location+Vector2f(xx*16.0f,yy*16.0f));

                float tile_position_x = std::floor(check_location.x/16.0f+xx) * 16.0f;
                float tile_position_y = std::floor(check_location.y/16.0f+yy) * 16.0f;

                if(ufoMaths::RectangleVsRectangle(Rectangle(check_location, Vector2f(14.0f,14.0f)), Rectangle(Vector2f(tile_position_x, tile_position_y), Vector2f(16.0f, 16.0f)))){
                    if(tile_id == 0){
                        z_index = 4;
                        
                    }
                    else{
                        z_index = 1;
                        stop_iterating = true;
                        break;
                    }
                }
            }
            if(stop_iterating) break;
        }
    }

    if(former_z_index != z_index) level->should_resort_after_z_index = true;
}

void
FloatingBlock::OnHit(TuxTalePlayer* _level){

}

void
FloatingBlock::OnOutOfRange(TuxTalePlayer* _player){

}

void
FloatingBlock::OnInRange(TuxTalePlayer* _player){

}

std::string FloatingBlock::GetType(){
    return "FloatingBlock";
}