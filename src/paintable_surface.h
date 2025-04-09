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

    float brush_radius = 10.0f;

    Level* level = nullptr;

    PaintableSurface(Vector2f _) : Actor(Vector2f(0.0f,0.0f)){
        
    }

    void OnLevelEnter(Level* _level){
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
        level = _level;

        visual_surface_ref->visible = false;

        //Load patterned textures
        AssetManager::Get().LoadDecal("../res/assets/sample_texture_grass.png","sample_texture_grass");
    }

    void ApplyPattern(olc::Decal* _visual_decal, olc::Decal* _sampling_decal, int _x, int _y){
        _visual_decal->sprite->SetPixel(_x,_y,_sampling_decal->sprite->GetPixel(_x%(_sampling_decal->sprite->Size().x),_y%(_sampling_decal->sprite->Size().x)));
    }

    void OnUpdate(){
        if(SingleKeyboard::Get().GetKey(olc::V).is_pressed){
            visual_surface_ref->visible = !visual_surface_ref->visible;
            layer_separation_surface_ref->visible = !layer_separation_surface_ref->visible;
        }

        Vector2f world_mouse_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition());

        int scroll_direction = Mouse::Get().GetScrollDirection();
        brush_radius += scroll_direction /** Engine::Get().GetDeltaTime()*/;
        if(brush_radius < 1.0f) brush_radius = 1.0f;

        if(Mouse::Get().GetLeftButton().is_held){
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
                
                }
            }
        }

        layer_separation_surface->Update();
        visual_surface->Update();
    }

};