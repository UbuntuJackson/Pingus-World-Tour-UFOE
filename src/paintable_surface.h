#pragma once
#include <actor.h>
#include <sprite_reference.h>
#include <olcPixelGameEngine.h>
#include <level.h>
#include <mouse.h>
#include <ufo_maths.h>
#include <graphics.h>
#include <ufo_engine.h>
#include <memory>
#include <console.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb_image_write.h"
#include "pingus_level.h"

/// @spawn;
class PaintableSurface : public Actor{
public:
    Colour CRUST = Colour(250,250,250);
    Colour CRUST_DARK = Colour(200,200,200);
    Colour MANTLE = Colour(150,150,150);

    Colour CRUST_VISUAL = Colour(0,200,80);
    Colour CRUST_DARK_VISUAL = Colour(0,100,40);
    Colour MANTLE_VISUAL = Colour(200,200,80);

    olc::Decal* layer_separation_surface;
    SpriteReference* layer_separation_surface_ref = nullptr;
    olc::Decal* visual_surface;
    SpriteReference* visual_surface_ref = nullptr;

    float brush_radius = 20.0f;

    PingusLevel* level = nullptr;

    PaintableSurface(Vector2f _) : Actor(Vector2f(0.0f,0.0f)){
        
    }

    void OnStart(Level* _level){
        AssetManager::Get().sprites["layer_separation_surface"] = std::make_unique<olc::Sprite>(_level->level_size.x, _level->level_size.y);
        AssetManager::Get().decals["layer_separation_surface"] = std::make_unique<olc::Decal>(AssetManager::Get().GetSprite("layer_separation_surface"));
        layer_separation_surface = AssetManager::Get().GetDecal("layer_separation_surface");
        layer_separation_surface_ref = AddChild<SpriteReference>("layer_separation_surface",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(_level->level_size.x, _level->level_size.y),
            Vector2f(1.0f,1.0f),
            0.0f
        );

        for(int yy = 0; yy < _level->level_size.y; yy++){
            for(int xx = 0; xx < _level->level_size.x; xx++){
                layer_separation_surface->sprite->SetPixel(xx,yy,Colour(0,0,0,0));
            }
        }

        AssetManager::Get().sprites["visual_surface"] = std::make_unique<olc::Sprite>(_level->level_size.x, _level->level_size.y);
        AssetManager::Get().decals["visual_surface"] = std::make_unique<olc::Decal>(AssetManager::Get().GetSprite("visual_surface"));
        visual_surface = AssetManager::Get().GetDecal("visual_surface");
        visual_surface_ref = AddChild<SpriteReference>("visual_surface",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(_level->level_size.x, _level->level_size.y),
            Vector2f(1.0f,1.0f),
            0.0f
        );

        for(int yy = 0; yy < _level->level_size.y; yy++){
            for(int xx = 0; xx < _level->level_size.x; xx++){
                visual_surface->sprite->SetPixel(xx,yy,Colour(0,0,0,0));
            }
        }

        level = dynamic_cast<PingusLevel*>(_level);

        visual_surface_ref->visible = false;

        level->is_menu = true;

        //Load patterned textures
        AssetManager::Get().LoadDecal("../res/assets/sample_texture_grass.png","sample_texture_grass");
    }

    void ApplyPattern(olc::Decal* _visual_decal, olc::Decal* _sampling_decal, int _x, int _y){
        _visual_decal->sprite->SetPixel(_x,_y,_sampling_decal->sprite->GetPixel(_x%(_sampling_decal->sprite->Size().x),_y%(_sampling_decal->sprite->Size().x)));
    }

    void SaveImage(const std::string& _path, olc::Decal* _decal){
        int w = _decal->sprite->Size().x;
        int h = _decal->sprite->Size().y;
        unsigned char* data = new unsigned char[w*h*3];
        int index = 0;
        for(int yy = 0; yy < h; yy++){
            for(int xx = 0; xx < w; xx++){
                int r = _decal->sprite->GetPixel(xx,yy).r;
                int g = _decal->sprite->GetPixel(xx,yy).g;
                int b = _decal->sprite->GetPixel(xx,yy).b;
                data[index++] = r;
                data[index++] = g;
                data[index++] = b;
            }
        }

        delete[] data;

        int save_success = stbi_write_png(
            _path.c_str(),
            _decal->sprite->Size().x,
            _decal->sprite->Size().y, 3, (void*)(data), w * 3);
        Console::PrintLine("PaintableSurface::OnUpdate: Saved",_path,"return value:",save_success);

        
    }

    bool CanFillPixel(Vector2f _marching_position){
        return layer_separation_surface->sprite->GetPixel(_marching_position) != MANTLE && ufoMaths::RectangleVsPoint(ufo::Rectangle(Vector2f(0.0f,0.0f),level->level_size), _marching_position);
    }

    void OnUpdate(){
        if(SingleKeyboard::Get().GetKey(olc::S).is_pressed){
            SaveImage("../res/game_generated_terrain/drawn_terrain_example.png",visual_surface);
            SaveImage("../res/game_generated_terrain/drawn_terrain_example_layer_separation.png",layer_separation_surface);
        }

        if(SingleKeyboard::Get().GetKey(olc::V).is_pressed){
            visual_surface_ref->visible = !visual_surface_ref->visible;
            layer_separation_surface_ref->visible = !layer_separation_surface_ref->visible;
        }

        Vector2f world_mouse_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition());

        int scroll_direction = Mouse::Get().GetScrollDirection();
        brush_radius += scroll_direction /** Engine::Get().GetDeltaTime()*/;
        if(brush_radius < 1.0f) brush_radius = 1.0f;

        if(SingleKeyboard::Get().GetKey(olc::F).is_held && Mouse::Get().GetLeftButton().is_pressed){
            Vector2f marching_position = world_mouse_position;
            while(CanFillPixel(marching_position)){
                while(CanFillPixel(marching_position)){
                    layer_separation_surface->sprite->SetPixel(marching_position, MANTLE);
                    marching_position.y+=1.0f;
                    Console::PrintLine("PaintableSurface::OnUpdate while loop");
                }
                marching_position.y = world_mouse_position.y;
                marching_position.x += 1.0f;
            }
            marching_position = world_mouse_position+Vector2f(0.0f,-1.0f);
            while(CanFillPixel(marching_position)){
                while(CanFillPixel(marching_position)){
                    layer_separation_surface->sprite->SetPixel(marching_position, MANTLE);
                    marching_position.y-=1.0f;
                    Console::PrintLine("PaintableSurface::OnUpdate while loop");
                }
                marching_position.y = world_mouse_position.y-1.0f;
                marching_position.x += 1.0f;
            }
            marching_position = world_mouse_position+Vector2f(-1.0f,-2.0f);
            while(CanFillPixel(marching_position)){
                while(CanFillPixel(marching_position)){
                    layer_separation_surface->sprite->SetPixel(marching_position, MANTLE);
                    marching_position.y+=1.0f;
                    Console::PrintLine("PaintableSurface::OnUpdate while loop");
                }
                marching_position.y = world_mouse_position.y-2.0f;
                marching_position.x -= 1.0f;
            }
            marching_position = world_mouse_position+Vector2f(-2.0f,-3.0f);
            while(CanFillPixel(marching_position)){
                while(CanFillPixel(marching_position)){
                    layer_separation_surface->sprite->SetPixel(marching_position, MANTLE);
                    marching_position.y-=1.0f;
                    Console::PrintLine("PaintableSurface::OnUpdate while loop");
                }
                marching_position.y = world_mouse_position.y-3.0f;
                marching_position.x -= 1.0f;
            }
            for(int yy = 0; yy < level->level_size.y; yy++){
                for(int xx = 0; xx < level->level_size.x; xx++){
                    float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                    
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST_DARK){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_DARK_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == MANTLE){
                        visual_surface->sprite->SetPixel(xx,yy,MANTLE_VISUAL);
                        ApplyPattern(visual_surface,AssetManager::Get().GetDecal("sample_texture_grass"),xx,yy);
                    }
                
                }
            }
        }

        if(Mouse::Get().GetLeftButton().is_held && !SingleKeyboard::Get().GetKey(olc::F).is_held){
            if(!SingleKeyboard::Get().GetKey(olc::E).is_held){
                for(int yy = world_mouse_position.y - brush_radius; yy < world_mouse_position.y + brush_radius; yy++){
                    for(int xx = world_mouse_position.x - brush_radius; xx < world_mouse_position.x + brush_radius; xx++){
                        float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                        if(dist < brush_radius){
                            if(layer_separation_surface->sprite->GetPixel(xx,yy) != MANTLE && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST_DARK) layer_separation_surface->sprite->SetPixel(xx,yy,CRUST);

                            if(dist < brush_radius - 2.0f && layer_separation_surface->sprite->GetPixel(xx,yy) != MANTLE){
                                layer_separation_surface->sprite->SetPixel(xx,yy,CRUST_DARK);
                            }

                            if(dist < brush_radius-3.8f){
                                layer_separation_surface->sprite->SetPixel(xx,yy,MANTLE);
                            }

                        }
                    
                    }
                }
            }
            else{

                for(int yy = world_mouse_position.y - brush_radius-5.0f; yy < world_mouse_position.y + brush_radius+5.0f; yy++){
                    for(int xx = world_mouse_position.x - brush_radius-5.0f; xx < world_mouse_position.x + brush_radius+5.0f; xx++){
                        float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                        
                        if(layer_separation_surface->sprite->GetPixel(xx,yy) == Colour(0,0,0,0)) continue;

                        if(dist < brush_radius+5.0f && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST_DARK){
                            layer_separation_surface->sprite->SetPixel(xx,yy,MANTLE);
                        }

                        if(dist < brush_radius+3.0f && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST){
                            layer_separation_surface->sprite->SetPixel(xx,yy,CRUST_DARK);
                        }

                        if(dist < brush_radius+2.0f){
                            layer_separation_surface->sprite->SetPixel(xx,yy,CRUST);
                        }

                        if(dist < brush_radius){
                            layer_separation_surface->sprite->SetPixel(xx,yy,Colour(0,0,0,0));
                        }

                        
                    
                    }
                }
            
            }

            for(int yy = world_mouse_position.y - brush_radius*2; yy < world_mouse_position.y + brush_radius*2; yy++){
                for(int xx = world_mouse_position.x - brush_radius*2; xx < world_mouse_position.x + brush_radius*2; xx++){
                    float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                    
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST_DARK){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_DARK_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == MANTLE){
                        visual_surface->sprite->SetPixel(xx,yy,MANTLE_VISUAL);
                        ApplyPattern(visual_surface,AssetManager::Get().GetDecal("sample_texture_grass"),xx,yy);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == Colour(0,0,0,0)){
                        visual_surface->sprite->SetPixel(xx,yy,Colour(0,0,0,0));
                    }
                
                }
            }
        }

        layer_separation_surface->Update();
        visual_surface->Update();
    }

    void OnWidgetDraw(){
        Vector2f mouse_position = Mouse::Get().GetPosition();

        float one_degree = 2.0f*ufoMaths::PI/360.0f;

        for(int a = 0; a < 360; a++){
            Graphics::Get().DrawLine(mouse_position + Vector2f(std::cos(a*one_degree),std::sin(a*one_degree))*brush_radius*level->GetActiveCamera()->scale, mouse_position + Vector2f(std::cos((a+1)*one_degree),std::sin((a+1)*one_degree))*brush_radius*level->GetActiveCamera()->scale,Graphics::WHITE);
        }

    }

};