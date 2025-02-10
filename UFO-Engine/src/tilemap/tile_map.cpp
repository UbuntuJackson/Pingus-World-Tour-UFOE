#include <vector>
#include <memory>
#include "tile_map.h"
#include "../ufo_maths/ufo_maths.h"
#include "../actor/actor.h"
#include "../level/level.h"
#include "../json/json.h"
using namespace ufoMaths;

std::unique_ptr<TileMap>
TileMap::Load(Json& _layer){
    std::unique_ptr<TileMap> u_tilemap = std::make_unique<TileMap>();
    
    if(_layer.GetAsString("type") == "tilelayer" && _layer.GetAsString("name") != "tilemap_collision"){
        std::vector<int> data;
        auto layer_data = _layer.GetObject("data");

        Json::ArrayForEach(
            Json(),layer_data,[&](Json& _json, const Json& _layer_data){
                int id = _json.GetAsInt();
                u_tilemap->tilemap_data.push_back(id);
            }
        );

    }

    return std::move(u_tilemap);
}

int TileMap::GetTileID_AtLevelPosition(Vector2f _position){
    int tile_id = tilemap_data[int(_position.y/16.0f) * level->tilemap.number_of_columns + (_position.x/16.0f)];
}

void TileMap::OnDraw(Camera* _camera){
    float scale = _camera->scale;
    Bounds world_bounds = _camera->world;
    
    
    for(auto&& tileset : level->tilemap.tileset_data){
        Rectangle screen_rectangle = _camera->GetOnScreenRectangleInWorld({tileset.tile_width, tileset.tile_height});

        int tile_start_x = int(std::floor(screen_rectangle.position.x/tileset.tile_width));
        int tile_end_x = int(std::floor((screen_rectangle.position.x + screen_rectangle.size.x)/tileset.tile_width));
        int tile_start_y = int(std::floor(screen_rectangle.position.y/tileset.tile_height));
        int tile_end_y = int(std::floor((screen_rectangle.position.y + screen_rectangle.size.y)/tileset.tile_height));

        tile_start_x = std::max(tile_start_x, 0);
        tile_end_x = std::min(tile_end_x, level->tilemap.number_of_columns);
        tile_start_y = std::max(tile_start_y, 0);
        tile_end_y = std::min(tile_end_y, level->tilemap.number_of_rows);

        for(int index_y = tile_start_y; index_y < tile_end_y; index_y++){
            for(int index_x = tile_start_x; index_x < tile_end_x; index_x++){
                int tile_id = tilemap_data[index_y*level->tilemap.number_of_columns + index_x];
                
                olc::vd2d tile_position = {index_x*tileset.tile_width, index_y*tileset.tile_height};

                if(tileset.tileset_start_id <= tile_id && tile_id < tileset.tileset_start_id+tileset.tile_count){
                    Rectangle sample_rectangle = GetFrameFromSpriteSheet(tileset.name,tile_id-tileset.tileset_start_id,{tileset.tile_width, tileset.tile_height});
                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
                        _camera->Transform(tile_position),
                        AssetManager::Get().GetDecal(tileset.name),
                        0.0f,
                        {0.0f, 0.0f},
                        sample_rectangle.position,
                        sample_rectangle.size,
                        {scale, scale}
                    );
                }
                
            }
        }

    }
    
}