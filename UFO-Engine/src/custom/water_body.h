#ifndef WATER_BODY_H
#define WATER_BODY_H

#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../json/json.h"

#include "../shapes/shape.h"
#include "../shapes/rectangle.h"
#include "frogatto_level.h"
#include "../level/level.h"
#include "../camera/camera.h"
#include "../drawing_system/drawing_system.h"

class WaterBody : public Shape<Rectangle>{
public:
    WaterBody(olc::vf2d _local_position, olc::vf2d _size) : Shape<Rectangle>(Rectangle(olc::vf2d(0.0f, 0.0f), _size) ,_local_position){

    }

    void OnLevelEnter(Level* _level){
        FrogattoLevel* level = dynamic_cast<FrogattoLevel*>(_level);
        level->water_body_handles.push_back(this);
    }

    static std::unique_ptr<WaterBody> Load(Json& _json){
        float x = (float)_json.GetAsInt("x");
        float y = (float)_json.GetAsInt("y");
        float width = (float)_json.GetAsInt("width");
        float height = (float)_json.GetAsInt("height");
        
        return std::make_unique<WaterBody>(olc::vf2d(x,y), olc::vf2d(width, height));
    }

    void DrawWater(Camera* _camera){
        //DrawingSystem::DrawFilled(shape, _camera, olc::Pixel(0,0,200));

        float scale = _camera->scale;
        Bounds world_bounds = _camera->world;

        float tile_width = 8.0f;
        float tile_height = 8.0f;
        
        Rectangle screen_rectangle = _camera->GetOnScreenRectangleInWorld({tile_width, tile_height});

        float top_cut_off = 0.0f;
        float bottom_cut_off = 0.0f;
        float left_cut_off = 0.0f;
        float right_cut_off = 0.0f;

        top_cut_off = screen_rectangle.position.y - (shape.position.y);
        bottom_cut_off = (shape.position.y+shape.size.y) - (screen_rectangle.position.y+screen_rectangle.size.y);
        left_cut_off = screen_rectangle.position.x - (shape.position.x);
        right_cut_off = (shape.position.x+shape.size.x) - (screen_rectangle.position.x+screen_rectangle.size.x);

        float start_y = shape.position.y;
        float end_y = shape.position.y + shape.size.y;

        float start_x = shape.position.x;
        float end_x = shape.position.x + shape.size.x;

        //Console::Out(bottom_cut_off);

        start_y = std::max(start_y, start_y+top_cut_off);
        end_y = std::min(end_y, end_y-bottom_cut_off);
        start_x = std::max(start_x, start_x+left_cut_off);
        end_x = std::min(end_x, end_x-right_cut_off);

        Console::Out(bottom_cut_off);

        for(int index_y = end_y/8-1; index_y >= start_y/8; index_y--){
            for(int index_x = start_x/8; index_x < end_x/8; index_x++){
                
                olc::vd2d tile_position = {index_x*tile_width, index_y*tile_height};

                int tile_id = 10;

                int row = 2;

                if(top_cut_off < 0.0f){ //Top isn't cut off?
                    if(index_y == start_y/8){
                        tile_id = 1+index_x%2;
                        row = 0;
                    }
                    

                    
                }
                if(bottom_cut_off){
                    if(index_y == end_y/8-1){
                        tile_id = 14;
                        row = 3;
                    }
                }

                if(left_cut_off < 0.0f){ //Top isn't cut off?
                    if(index_x == start_x/8){
                        tile_id = 0+row*4;
                    }
                }
                if(right_cut_off < 0.0f){
                    if(index_x == end_x/8-1){
                        tile_id = 3+row*4;
                    }
                }

                if(row != 0){
                    Rectangle sample_rectangle = GetFrameFromSpriteSheet("under_water",tile_id,{tile_width, tile_height});
                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
                        _camera->Transform(tile_position),
                        AssetManager::Get().GetDecal("under_water"),
                        0.0f,
                        {0.0f, 0.0f},
                        sample_rectangle.position,
                        sample_rectangle.size,
                        {scale, scale}
                    );
                }
                else{
                    Rectangle sample_rectangle = GetFrameFromSpriteSheet("under_water",tile_id+4,{tile_width, tile_height});
                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    
                    olc::vf2d tile_position_2 = tile_position;
                    if(_camera->position.y > position.y){
                        tile_position_2.y += (std::abs(_camera->position.y-position.y))/100.0f*8.0f;
                    }

                    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
                        _camera->Transform(tile_position_2),
                        AssetManager::Get().GetDecal("under_water"),
                        0.0f,
                        {0.0f, 0.0f},
                        sample_rectangle.position,
                        sample_rectangle.size,
                        {scale, scale}
                    );

                    sample_rectangle = GetFrameFromSpriteSheet("under_water",tile_id,{tile_width, tile_height});

                    //Console::Out("sample rectangle:", sample_rectangle.position, sample_rectangle.size);
                    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
                        _camera->Transform(tile_position),
                        AssetManager::Get().GetDecal("under_water"),
                        0.0f,
                        {0.0f, 0.0f},
                        sample_rectangle.position,
                        sample_rectangle.size,
                        {scale, (_camera->position.y-position.y)/100.0f}
                    );
                }
                
                
            }
        }

            
        
    }
};

#endif