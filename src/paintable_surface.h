#pragma once
#include <actor.h>
#include <sprite_reference.h>
#include <olcPixelGameEngine.h>
#include <level.h>
#include <mouse.h>
#include <ufo_maths.h>
#include <graphics.h>
#include <ufo_engine.h>
#include <single_keyboard.h>
#include <memory>
#include <cmath>
#include <console.h>
#include "../external/stb_image_write.h"
#include "pingus_level.h"
#include <level_sprite_reference.h>
#include "pingus_world_tour_editor.h"
#include "colour_picker.h"

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

    std::string current_texture_key = "light_house_rocks";

    PingusLevel* level = nullptr;
    std::string path = "";
    std::string visual_suface_name = "";
    std::string layer_separation_surface_name = "";

    PingusWorldTourEditor* editor = nullptr;

    PaintableSurface(PingusWorldTourEditor* _editor, std::string _visual_surface_name, std::string _path) :
    Actor(Vector2f(0.0f,0.0f)),
    editor{_editor},
    visual_suface_name{_visual_surface_name},
    layer_separation_surface_name{_visual_surface_name+"_layer_separation"},
    path{_path}{
        
    }

    void OnStart(Level* _level){

        level = dynamic_cast<PingusLevel*>(_level);

        //Instead of creating SpriteReference, create an actual PingusWorldTour LevelSpriteReference layer. Grab reference to that olcDecal
        level->asset_manager.sprites[layer_separation_surface_name] = std::make_unique<olc::Sprite>(_level->level_size.x, _level->level_size.y);
        level->asset_manager.decals[layer_separation_surface_name] = std::make_unique<olc::Decal>(level->asset_manager.GetSprite(layer_separation_surface_name));
        layer_separation_surface = level->asset_manager.GetDecal(layer_separation_surface_name);
        layer_separation_surface_ref = AddChild<LevelSpriteReference>(layer_separation_surface_name,
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

        level->asset_manager.sprites[visual_suface_name] = std::make_unique<olc::Sprite>(_level->level_size.x, _level->level_size.y);
        level->asset_manager.decals[visual_suface_name] = std::make_unique<olc::Decal>(level->asset_manager.GetSprite(visual_suface_name));
        visual_surface = level->asset_manager.GetDecal(visual_suface_name);
        visual_surface_ref = AddChild<LevelSpriteReference>(visual_suface_name,
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

        visual_surface_ref->visible = false;

        level->is_menu = true;

        //Load patterned textures
        AssetManager::Get().LoadDecal("../res/assets/sample_texture_grass.png","sample_texture_grass");
        AssetManager::Get().LoadDecal("../res/assets/lighthouse_rocks.png","light_house_rocks");
    }

    void ApplyPattern(olc::Decal* _visual_decal, olc::Decal* _sampling_decal, int _x, int _y){
        _visual_decal->sprite->SetPixel(_x,_y,_sampling_decal->sprite->GetPixel(_x%(_sampling_decal->sprite->Size().x),_y%(_sampling_decal->sprite->Size().x)));
    }

    //Good as it is, since you can call SaveImage on each PaintableSurface
    void SaveImage(const std::string& _path, olc::Decal* _decal){
        int w = _decal->sprite->Size().x;
        int h = _decal->sprite->Size().y;
        unsigned char* data = new unsigned char[w*h*4];
        int index = 0;
        for(int yy = 0; yy < h; yy++){
            for(int xx = 0; xx < w; xx++){
                Colour p = _decal->sprite->GetPixel(xx,yy);
                data[index++] = p.r;
                data[index++] = p.g;
                data[index++] = p.b;
                data[index++] = p.a;
            }
        }

        int save_success = stbi_write_png(
            _path.c_str(),
            w,
            h, 4, data, w * 4);
        Console::PrintLine("PaintableSurface::OnUpdate: Saved",_path,"return value:",save_success);

        delete[] data;
        
    }

    bool CanFillPixel(Vector2f _marching_position){
        return layer_separation_surface->sprite->GetPixel(_marching_position) != MANTLE && ufoMaths::RectangleVsPoint(ufo::Rectangle(Vector2f(0.0f,0.0f),level->level_size), _marching_position);
    }

    //Change this to LayerSelectedUpdate
    void SelectedUpdate(){
        if(SingleKeyboard::Get().GetKey(olc::S).is_pressed){
            SaveImage("../res/game_generated_terrain/drawn_terrain_example.png",visual_surface);
            SaveImage("../res/game_generated_terrain/drawn_terrain_example_layer_separation.png",layer_separation_surface);
        }

        if(SingleKeyboard::Get().GetKey(olc::V).is_pressed){
            visual_surface_ref->visible = !visual_surface_ref->visible;
            layer_separation_surface_ref->visible = !layer_separation_surface_ref->visible;
        }

        Vector2f world_mouse_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition());

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
                        ApplyPattern(visual_surface,AssetManager::Get().GetDecal(current_texture_key),xx,yy);
                    }
                
                }
            }
        }

        if(Mouse::Get().GetLeftButton().is_held && !SingleKeyboard::Get().GetKey(olc::F).is_held){
            if(!SingleKeyboard::Get().GetKey(olc::E).is_held){
                for(int yy = world_mouse_position.y - editor->brush_radius; yy < world_mouse_position.y + editor->brush_radius; yy++){
                    for(int xx = world_mouse_position.x - editor->brush_radius; xx < world_mouse_position.x + editor->brush_radius; xx++){
                        float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                        if(dist < editor->brush_radius){
                            if(layer_separation_surface->sprite->GetPixel(xx,yy) != MANTLE && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST_DARK) layer_separation_surface->sprite->SetPixel(xx,yy,CRUST);

                            if(dist < editor->brush_radius - 2.0f && layer_separation_surface->sprite->GetPixel(xx,yy) != MANTLE){
                                layer_separation_surface->sprite->SetPixel(xx,yy,CRUST_DARK);
                            }

                            if(dist < editor->brush_radius-3.8f){
                                layer_separation_surface->sprite->SetPixel(xx,yy,MANTLE);
                            }

                        }
                    
                    }
                }
            }
            else{

                for(int yy = world_mouse_position.y - editor->brush_radius-5.0f; yy < world_mouse_position.y + editor->brush_radius+5.0f; yy++){
                    for(int xx = world_mouse_position.x - editor->brush_radius-5.0f; xx < world_mouse_position.x + editor->brush_radius+5.0f; xx++){
                        float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                        
                        if(layer_separation_surface->sprite->GetPixel(xx,yy) == Colour(0,0,0,0)) continue;

                        if(dist < editor->brush_radius+5.0f && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST_DARK){
                            layer_separation_surface->sprite->SetPixel(xx,yy,MANTLE);
                        }

                        if(dist < editor->brush_radius+3.5f && layer_separation_surface->sprite->GetPixel(xx,yy) != CRUST){
                            layer_separation_surface->sprite->SetPixel(xx,yy,CRUST_DARK);
                        }

                        if(dist < editor->brush_radius+2.0f){
                            layer_separation_surface->sprite->SetPixel(xx,yy,CRUST);
                        }

                        if(dist < editor->brush_radius){
                            layer_separation_surface->sprite->SetPixel(xx,yy,Colour(0,0,0,0));
                        }

                        
                    
                    }
                }
            
            }

            for(int yy = world_mouse_position.y - editor->brush_radius*2; yy < world_mouse_position.y + editor->brush_radius*2; yy++){
                for(int xx = world_mouse_position.x - editor->brush_radius*2; xx < world_mouse_position.x + editor->brush_radius*2; xx++){
                    float dist = ufoMaths::Distance2(Vector2f(xx,yy),world_mouse_position);
                    
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST_DARK){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_DARK_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == MANTLE){
                        visual_surface->sprite->SetPixel(xx,yy,MANTLE_VISUAL);
                        ApplyPattern(visual_surface,AssetManager::Get().GetDecal(current_texture_key),xx,yy);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == Colour(0,0,0,0)){
                        visual_surface->sprite->SetPixel(xx,yy,Colour(0,0,0,0));
                    }
                
                }
            }
        }

        if(editor->colour_picker->SetColour() || editor->colour_picker->SetHue()){
            if(editor->b_upper_crust_colour->is_selected) CRUST_VISUAL = editor->colour_picker->GetColour();
            if(editor->b_lower_crust_colour->is_selected) CRUST_DARK_VISUAL = editor->colour_picker->GetColour();
            //if(editor->b_upper_crust_colour) CRUST_VISUAL = editor->colour_picker->hue;
            for(int yy = 0; yy < level->level_size.y; yy++){
                for(int xx = 0; xx < level->level_size.x; xx++){
                    
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == CRUST_DARK){
                        visual_surface->sprite->SetPixel(xx,yy,CRUST_DARK_VISUAL);
                    }
                    if(layer_separation_surface->sprite->GetPixel(xx,yy) == MANTLE){
                        visual_surface->sprite->SetPixel(xx,yy,MANTLE_VISUAL);
                        ApplyPattern(visual_surface,AssetManager::Get().GetDecal(current_texture_key),xx,yy);
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

    void OnUpdate(){
        
    }

};