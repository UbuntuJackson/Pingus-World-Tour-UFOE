#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include <sprite_reference.h>
#include <graphics.h>
#include <camera.h>
#include <mouse.h>
#include <json_variant.h>
#include <ufo_engine.h>
#include <widget_sprite_reference.h>
#include <level_widget_sprite_reference.h>
#include <label.h>
#include <colour_rectangle_theme.h>
#include "world_map_location.h"
#include "pingus_level.h"
#include "world_map.h"

void WorldMapLocation::OnLevelEnter(Level* _level){
    spr_bg = AddChild<SpriteReference>(
        "locationpinbg",
        Vector2f(-1.0f,-34.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(34.0f,34.0f),
        Vector2f(1.0f,1.0f),
        0.0f
    );

    spr = AddChild<SpriteReference>(
        "red_x",
        Vector2f(0.0f,-32.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(1.0f,1.0f),
        0.0f
    );
    
    level = dynamic_cast<PingusLevel*>(_level);
    level->world_map_location_handles.push_back(this);
}

void WorldMapLocation::OnStart(Level* _level){
    JsonDictionary& unlocked_levels = Engine::Get().GetActiveProfile()->save_file.Get("unlocked_levels").AsDictionary();
    if(!unlocked_levels.Get(level_path).IsNull()) unlocked = true;
    
    if(unlocked){
        JsonVariant& level_data = Engine::Get().GetActiveProfile()->save_file.Get("cleared_levels").AsDictionary().Get(level_path);

        if(!level_data.IsNull()){
            switch(level_data.AsDictionary().Get("rank").AsInt()){
                case 0:
                    best_rank = "S";
                    rank_number = 3;
                    break;
                case 1:
                    best_rank = "A";
                    rank_number = 2;
                    break;
                case 2:
                    best_rank = "B";
                    rank_number = 1;
                    break;
                case 3:
                    best_rank = "L";
                    rank_number = 0;
                    break;
            }

            most_rescued_pingus = level_data.AsDictionary().Get("most_rescued_pingus").AsInt();
        }

        spr->current_frame_index = rank_number;

    }

    Console::PrintLine("Location",_level->path,unlocked);

    spr->visible = unlocked;
    spr_bg->visible = spr->visible;

    for(const auto& location : level->world_map_location_handles){
        if(location->level_path == unlocks_other_level){
            other_location = location;
        }
    }

    description_label = AddChild<Label>(Vector2f(460.0f,290.0f)-local_position, Vector2f(680.0f-480.0f, 480.0f-240.0f), description);
    ColourRectangleTheme* label_theme = dynamic_cast<ColourRectangleTheme*>(description_label->theme.get());
    label_theme->colour = Colour(0,0,0,0);
    description_label->text_wrapping_mode = Widget::TextWrappingModes::WORD_MEETS_BORDER;

    spr_original_position = spr->local_position;
}

void WorldMapLocation::OnUpdate(){

    if(level->world_map == nullptr) return;

    spr->local_position = spr_original_position;
    if(selected && unlocked){
        spr->local_position = spr_original_position - Vector2f(0.0f, 2.0f);
        float text_x = 0.0f;
        float screen_width_half = 340.0f;
        if(!level->world_map->selected_to_left) text_x = 20.0f;
        else text_x = 460.0f;

        f_tint += 600.0f* Engine::Get().GetDeltaTime();
        if(f_tint > 255.0f) f_tint = 255.0f;
        if(preview_image){
            preview_image->tint.a = (int)f_tint;
        }
        if(preview != "" && !preview_image){

            Console::PrintLine("Instantiated preview image");
            level->asset_manager.LoadDecal(preview,preview);
            preview_image = level->NewActor<LevelWidgetSpriteReference>(preview,
            Vector2f(text_x,20.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(200.0f,200.0f),
            Vector2f(1.0f,1.0f),0.0f);
            preview_image->tint.a = 0.0f;
            
        }
    }
    else if(preview_image){
        f_tint = 0.0f;
        Console::PrintLine("Should purge preview_image");
        preview_image->QueueForPurge();
        preview_image = nullptr;
    }
    spr_bg->local_position = spr->local_position + Vector2f(-1.0f,-2.0f);

    was_selected = selected;
}

void WorldMapLocation::OnDraw(Camera* _camera){
    if(other_location != nullptr){
        float distance = ufoMaths::Distance2(GetGlobalPosition(), other_location->GetGlobalPosition());

        if(distance < 30.0f) distance = 30.0f;

        for(int i = 20; i < (int)distance - 40; i++){
            
            if(i % 30 == 0) {
                Console::PrintLine("WorldMapLocation", i);
                
                Vector2f p0 = GetGlobalPosition() + (float)i * (other_location->GetGlobalPosition() - GetGlobalPosition()).norm();
                Vector2f p1 = GetGlobalPosition() + ((float)i+1) * (other_location->GetGlobalPosition() - GetGlobalPosition()).norm();

                float angle = std::atan2(p1.y - p0.y, p1.x - p0.x);

                Vector2f offset = Vector2f(16.0f,-16.0f);

                //Console::PrintLine("Line:", _camera->Transform(p0 + offset), _camera->Transform(p1 + offset));

                if(other_location->unlocked){
                    Graphics::Get().DrawLine( _camera->Transform(p0 + offset), _camera->Transform(p1 + offset), Graphics::RED);
                    Graphics::Get().DrawFrame(AssetManager::Get(), "dash", _camera->Transform(p0+offset), Vector2f(16.0f, 16.0f), Vector2f(32.0f, 32.0f), Vector2f(1.0f, 1.0f), 0, angle, olc::WHITE);
                }
            }
        }
        //Graphics::Get().DrawLine(GetGlobalPosition(), other_location->GetGlobalPosition(),Graphics::RED);
    }
    if(selected && unlocked){
        spr->tint = olc::WHITE;
        
    }
    else{
        spr->tint = olc::GREY;
    }
    spr_bg->tint = spr->tint;
}

void WorldMapLocation::OnWidgetDraw(){
    
    if(selected && unlocked){
        float screen_width_half = 340.0f;
        float text_x = 0.0f;
        if(!level->world_map->selected_to_left) text_x = 20.0f;
        else text_x = 460.0f;

        Graphics::Get().DrawString(Vector2f(text_x,240.0f), name_of_location, Graphics::WHITE ,{1.0f,1.0f});
        description_label->visible = true;
        description_label->local_position.x = text_x-local_position.x;
        Graphics::Get().DrawString(Vector2f(text_x,260.0f), "Best rank: "+best_rank, Graphics::WHITE ,{1.0f,1.0f});
        Graphics::Get().DrawString(Vector2f(text_x,270.0f), "Most rescued pingus: "+std::to_string(most_rescued_pingus), Graphics::WHITE ,{1.0f,1.0f});
    }
    else{
        description_label->visible = false;
    }
}

void WorldMapLocation::Unlock(){
    spr->visible = true;
    unlocked = true;
}