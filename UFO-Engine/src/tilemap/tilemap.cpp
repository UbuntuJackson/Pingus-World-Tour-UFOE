#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include "tilemap.h"
#include "tileset_data.h"
#include "../external/olcPixelGameEngine.h"
#include "../asset_manager/get_rect_from_index.h"
#include "../camera/camera.h"
#include "../ufo_engine/ufo_engine.h"
#include "../console/console.h"
#include "../json/json.h"

void Tilemap::Load(const Json& _json){

}

TilesetData Tilemap::GetTilesetData(std::string _name){
    for(auto&& data : tileset_data){
        if(data.name == _name) return data;
    }
    Console::Out("Tilemap::GetTilesetData Invalid tileset name:", _name);
    return {
        ""
    };
}

void Tilemap::Draw(Camera* _camera){
    float scale = _camera->scale;
    Bounds world_bounds = _camera->world;
    
    for(auto&& tiles : layer_data_sets){
        for(auto&& tileset : tileset_data){
            ufo::Rectangle screen_rectangle = _camera->GetOnScreenRectangleInWorld({tileset.tile_width, tileset.tile_height});

            int tile_start_x = int(std::floor(screen_rectangle.position.x/tileset.tile_width));
            int tile_end_x = int(std::floor((screen_rectangle.position.x + screen_rectangle.size.x)/tileset.tile_width));
            int tile_start_y = int(std::floor(screen_rectangle.position.y/tileset.tile_height));
            int tile_end_y = int(std::floor((screen_rectangle.position.y + screen_rectangle.size.y)/tileset.tile_height));

            tile_start_x = std::max(tile_start_x, 0);
            tile_end_x = std::min(tile_end_x, number_of_columns);
            tile_start_y = std::max(tile_start_y, 0);
            tile_end_y = std::min(tile_end_y, number_of_rows);

            for(int index_y = tile_start_y; index_y < tile_end_y; index_y++){
                for(int index_x = tile_start_x; index_x < tile_end_x; index_x++){
                    int tile_id = tiles[index_y*number_of_columns + index_x];
                    
                    olc::vd2d tile_position = {index_x*tileset.tile_width, index_y*tileset.tile_height};

                    if(tileset.tileset_start_id <= tile_id && tile_id < tileset.tileset_start_id+tileset.tile_count){
                        ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(tileset.name,tile_id-tileset.tileset_start_id,{tileset.tile_width, tileset.tile_height});
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
    /*
    for(auto&& tiles : layer_data_sets){

        for(int i = 0; i < tiles.size(); i++){
            int tile_id = tiles[i];
            olc::vf2d tile_col_row = {
                i%tilemap_width,
                std::floor(i/tilemap_width)
            };

            for(auto&& tileset : tileset_data){
                if(tileset.tileset_start_id <= tile_id && tile_id < tileset.tileset_start_id+tileset.tile_count){
                    Rectangle sample_rectangle = GetFrameFromSpriteSheet(tileset.name,tile_id-tileset.tileset_start_id,{tileset.tile_width, tileset.tile_height});
                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    GraphicsProvider::Get().DrawPartialRotatedDecal(
                        _camera->Transform(olc::vf2d(tileset.tile_width*tile_col_row.x, tileset.tile_height*tile_col_row.y)),
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
    */
}

void Tilemap::DrawCollisionTiles(Camera* _camera){
    if(tilemap_collision_data.size() == 0) return;
    float scale = _camera->scale;
    Bounds world_bounds = _camera->world;
    
    
    for(auto&& tileset : tileset_data){
        ufo::Rectangle screen_rectangle = _camera->GetOnScreenRectangleInWorld({tileset.tile_width, tileset.tile_height});

        int tile_start_x = int(std::floor(screen_rectangle.position.x/tileset.tile_width));
        int tile_end_x = int(std::floor((screen_rectangle.position.x + screen_rectangle.size.x)/tileset.tile_width));
        int tile_start_y = int(std::floor(screen_rectangle.position.y/tileset.tile_height));
        int tile_end_y = int(std::floor((screen_rectangle.position.y + screen_rectangle.size.y)/tileset.tile_height));

        tile_start_x = std::max(tile_start_x, 0);
        tile_end_x = std::min(tile_end_x, int(std::floor(world_bounds.x1/tileset.tile_width)));
        tile_start_y = std::max(tile_start_y, 0);
        tile_end_y = std::min(tile_end_y, int(std::floor(world_bounds.y1/tileset.tile_height)));

        for(int index_y = tile_start_y; index_y < tile_end_y; index_y++){
            for(int index_x = tile_start_x; index_x < tile_end_x; index_x++){
                int tile_id = tilemap_collision_data[index_y*number_of_columns + index_x];
                
                olc::vd2d tile_position = {index_x*tileset.tile_width, index_y*tileset.tile_height};

                if(tileset.tileset_start_id <= tile_id && tile_id < tileset.tileset_start_id+tileset.tile_count){
                    ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(tileset.name,tile_id-tileset.tileset_start_id,{tileset.tile_width, tileset.tile_height});
                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
                        _camera->Transform(tile_position),
                        AssetManager::Get().GetDecal(tileset.name),
                        0.0f,
                        {0.0f, 0.0f},
                        sample_rectangle.position,
                        sample_rectangle.size,
                        {scale, scale},
                        olc::Pixel(200,100,200,100)
                    );
                }
                
            }
        }

    }
    
    /*
    for(auto&& tiles : layer_data_sets){

        for(int i = 0; i < tiles.size(); i++){
            int tile_id = tiles[i];
            olc::vf2d tile_col_row = {
                i%tilemap_width,
                std::floor(i/tilemap_width)
            };

            for(auto&& tileset : tileset_data){
                if(tileset.tileset_start_id <= tile_id && tile_id < tileset.tileset_start_id+tileset.tile_count){
                    Rectangle sample_rectangle = GetFrameFromSpriteSheet(tileset.name,tile_id-tileset.tileset_start_id,{tileset.tile_width, tileset.tile_height});
                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    GraphicsProvider::Get().DrawPartialRotatedDecal(
                        _camera->Transform(olc::vf2d(tileset.tile_width*tile_col_row.x, tileset.tile_height*tile_col_row.y)),
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
    */
}

void Tilemap::DrawForeGround(Camera* _camera){

    float scale = _camera->scale;
    Bounds world_bounds = _camera->world;
    
    for(auto&& tiles : foreground_layer_data_sets){
        for(auto&& tileset : tileset_data){
            ufo::Rectangle screen_rectangle = _camera->GetOnScreenRectangleInWorld({tileset.tile_width, tileset.tile_height});

            int tile_start_x = int(std::floor(screen_rectangle.position.x/tileset.tile_width));
            int tile_end_x = int(std::floor((screen_rectangle.position.x + screen_rectangle.size.x)/tileset.tile_width));
            int tile_start_y = int(std::floor(screen_rectangle.position.y/tileset.tile_height));
            int tile_end_y = int(std::floor((screen_rectangle.position.y + screen_rectangle.size.y)/tileset.tile_height));

            tile_start_x = std::max(tile_start_x, 0);
            tile_end_x = std::min(tile_end_x, int(std::floor(world_bounds.x1/tileset.tile_width)));
            tile_start_y = std::max(tile_start_y, 0);
            tile_end_y = std::min(tile_end_y, int(std::floor(world_bounds.y1/tileset.tile_height)));

            for(int index_y = tile_start_y; index_y < tile_end_y; index_y++){
                for(int index_x = tile_start_x; index_x < tile_end_x; index_x++){
                    int tile_id = tiles[index_y*number_of_columns + index_x];
                    
                    olc::vd2d tile_position = {index_x*tileset.tile_width, index_y*tileset.tile_height};

                    if(tileset.tileset_start_id <= tile_id && tile_id < tileset.tileset_start_id+tileset.tile_count){
                        ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(tileset.name,tile_id-tileset.tileset_start_id,{tileset.tile_width, tileset.tile_height});
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
}