#ifndef TILEMAP_H
#define TILEMAP_H
#include <vector>
#include <unordered_map>
#include <string>
#include "tileset_data.h"
#include "../external/olcPixelGameEngine.h"
#include "../asset_manager/get_rect_from_index.h"
#include "../json/json.h"
#include "../actor/actor.h"

class Camera;

class Tilemap{
public:
    std::vector<std::vector<int>> layer_data_sets;
    std::vector<std::vector<int>> foreground_layer_data_sets;

    std::vector<int> tilemap_collision_data;
    
    int number_of_rows;
    int number_of_columns;
    int tile_width;
    int tile_height;
    std::vector<std::string> keys_to_temporary_assets;
    //  firstgid, key
    std::vector<TilesetData> tileset_data;

    TilesetData GetTilesetData(std::string _name);

    void Load(const Json& _json);

    Tilemap() = default;
    void Draw(Camera* _camera);
    void DrawForeGround(Camera* _camera);
    void DrawCollisionTiles(Camera* _camera);
};

#endif