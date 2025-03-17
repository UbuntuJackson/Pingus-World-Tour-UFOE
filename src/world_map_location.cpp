#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include <sprite_reference.h>
#include <graphics.h>
#include <camera.h>
#include <json_variant.h>
#include <ufo_engine.h>
#include <widget_sprite_reference.h>
#include <level_widget_sprite_reference.h>
#include <label.h>
#include <colour_rectangle_theme.h>
#include "world_map_location.h"
#include "pingus_level.h"

void WorldMapLocation::OnLevelEnter(Level* _level){
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
    
    Console::PrintLine("Location",_level->path,unlocked);

    spr->visible = unlocked;

    for(const auto& location : level->world_map_location_handles){
        if(location->level_path == unlocks_other_level){
            other_location = location;
        }
    }

    description_label = AddChild<Label>(Vector2f(460.0f,260.0f)-local_position, Vector2f(680.0f-480.0f, 480.0f-240.0f), description);
    ColourRectangleTheme* label_theme = dynamic_cast<ColourRectangleTheme*>(description_label->theme.get());
    label_theme->colour = Colour(0,0,0,0);
    description_label->text_wrapping_mode = Widget::TextWrappingModes::WORD_MEETS_BORDER;
}

void WorldMapLocation::OnUpdate(){
    if(selected && unlocked){
        f_tint += 600.0f* Engine::Get().GetDeltaTime();
        if(f_tint > 255.0f) f_tint = 255.0f;
        if(preview_image){
            preview_image->tint.a = (int)f_tint;
        }
        if(preview != "" && !preview_image){

            Console::PrintLine("Instantiated preview image");
            level->asset_manager.LoadDecal(preview,preview);
            preview_image = level->NewActor<LevelWidgetSpriteReference>(preview,
            Vector2f(680-220.0f,20.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(200.0f,200.0f),
            Vector2f(1.0f,1.0f),0.0f,0);
            preview_image->tint.a = 0.0f;
            
        }
    }
    else if(preview_image){
        f_tint = 0.0f;
        Console::PrintLine("Should purge preview_image");
        preview_image->QueueForPurge();
        preview_image = nullptr;
    }
}

void WorldMapLocation::OnDraw(Camera* _camera){
    if(other_location != nullptr){
        float distance = ufoMaths::Distance2(GetGlobalPosition(), other_location->GetGlobalPosition());

        for(int i = 0; i < (int)distance; i++){
            
            if( (i/20) % 2 ){
                
                Vector2f p0 = local_position + (float)i * (other_location->GetGlobalPosition() - GetGlobalPosition()).norm();
                Vector2f p1 = local_position + ((float)i+1) * (other_location->GetGlobalPosition() - GetGlobalPosition()).norm();

                Vector2f offset = Vector2f(16.0f,-16.0f);

                //Console::PrintLine("Line:", _camera->Transform(p0 + offset), _camera->Transform(p1 + offset));

                Graphics::Get().DrawLine( _camera->Transform(p0 + offset), _camera->Transform(p1 + offset), Graphics::RED);
            }
        }
        //Graphics::Get().DrawLine(GetGlobalPosition(), other_location->GetGlobalPosition(),Graphics::RED);
    }
    if(selected && unlocked){
        spr->tint = olc::WHITE;
        
    }
    else{
        spr->tint = olc::DARK_RED;
    }
}

void WorldMapLocation::OnWidgetDraw(){
    if(selected && unlocked){
        Graphics::Get().DrawString(Vector2f(460.0f,240.0f), name_of_location, Graphics::WHITE ,{1.0f,1.0f});
        description_label->visible = true;
        //Graphics::Get().DrawString(Vector2f(480.0f,240.0f), description, Graphics::WHITE ,{1.0f,1.0f});
    }
    else{
        description_label->visible = false;
    }
}

void WorldMapLocation::Unlock(){
    spr->visible = true;
    unlocked = true;
}