#include <widget.h>
#include <mouse.h>
#include <graphics.h>
#include <ufo_maths.h>
#include <button.h>
#include <level.h>
#include <wrap_menu.h>
#include "pingus_level.h"
#include "pingus_world_tour_editor.h"
#include "paintable_surface.h"
#include "pwt_editor_layer_button.h"
#include "colour_picker.h"
#include <text_field.h>
#include <file_dialogue.h>
#include <colour_rectangle_theme.h>
#include <theme.h>
#include "pingu.h"

PingusWorldTourEditor::PingusWorldTourEditor(Vector2f _) : Widget(Vector2f(0.0f,0.0f),Engine::Get().pixel_game_engine.GetWindowSizeInPixles()){}

void PingusWorldTourEditor::OnLevelEnter(Level* _level){
    Widget::OnLevelEnter(_level);
    level = dynamic_cast<PingusLevel*>(_level);

    b_save = AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(32.0f,0.0f),"Save");
    b_save->adjust_height_after_text_rows = true;
    b_load = AddChild<Button>(Vector2f(32.0f,0.0f),Vector2f(32.0f,0.0f),"Load");
    b_load->adjust_height_after_text_rows = true;
    b_import_layer = AddChild<Button>(Vector2f(32.0f*2,0.0f),Vector2f(56.0f,0.0f),"Import Layer");
    b_import_layer->adjust_height_after_text_rows = true;

    m_colour_settings = AddChild<WrapMenu>(Vector2f(GetRectangle().size.x-200.0f,100.0f),Vector2f(200.0f,200.0f));

    b_upper_crust_colour = m_colour_settings->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(64.0f,0.0f),"Upper crust colour");
    b_upper_crust_colour->adjust_height_after_text_rows = true;
    b_lower_crust_colour = m_colour_settings->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(64.0f,0.0f),"Lower cust colour");
    b_lower_crust_colour->adjust_height_after_text_rows = true;
    b_mantle_texture = m_colour_settings->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(64.0f,0.0f),"Mantle texture");
    b_mantle_texture->adjust_height_after_text_rows = true;

    m_solid_type = AddChild<WrapMenu>(Vector2f(GetRectangle().size.x-100.0f,100.0f),Vector2f(200.0f,200.0f));

    b_solid = m_solid_type->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(64.0f,0.0f),"Solid");
    b_solid->adjust_height_after_text_rows = true;
    b_semi_solid = m_solid_type->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(64.0f,0.0f),"Semi solid");
    b_semi_solid->adjust_height_after_text_rows = true;
    b_indestructible_solid = m_solid_type->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(64.0f,0.0f),"Indestructible");
    b_indestructible_solid->adjust_height_after_text_rows = true;

    m_layers = AddChild<WrapMenu>(Vector2f(GetRectangle().size.x-200.0f,0.0f),Vector2f(200.0f,200.0f));

    AddChild<Button>(GetRectangle().size-Vector2f(32.0f,32.0f), Vector2f(32.0f,32.0f), "S");
    b_spawn_pingu = AddChild<Button>(GetRectangle().size-Vector2f(64.0f,32.0f), Vector2f(32.0f,32.0f), "P");
    AddChild<Button>(GetRectangle().size-Vector2f(96.0f,32.0f), Vector2f(32.0f,32.0f), "C");

    AddLayer("mg", "mg");
    AddLayer("solid", "solid");
    
    level->is_menu = true;

    colour_picker = AddChild<ColourPicker>(Vector2f(GetRectangle().size.x-200.0f,200.0f),Vector2f(200.0f,232.0f));
}

void PingusWorldTourEditor::AddLayer(std::string _name, std::string _path){
    auto layer = level->NewActor<PaintableSurface>(this,_name,_path);
    m_layers->AddChild<PWTEditorLayerButton>(this,layer,Vector2f(0.0f,0.0f),Vector2f(200.0f,32.0f),_name);
}

void PingusWorldTourEditor::OnWidgetHovered(){
    int scroll_direction = Mouse::Get().GetScrollDirection();
    brush_radius += scroll_direction*1024.0f * Engine::Get().GetDeltaTime();
    if(brush_radius < 1.0f) brush_radius = 1.0f;

    if(selected_layer){
        selected_layer->SelectedUpdate();
    }
    else if(Mouse::Get().GetLeftButton().is_pressed){
        switch(spawn_mode){
            case SpawnModes::PINGU:
                level->NewActor<Pingu>(Engine::Get().current_level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()));
                break;      
        }
    }
}
void PingusWorldTourEditor::OnUpdate(){

    if(b_spawn_pingu->IsPressed()){
        selected_layer = nullptr;
    }

    if(b_solid->IsPressed()) selected_solid_type = SOLID;
    if(b_semi_solid->IsPressed()) selected_solid_type = SEMI_SOLID;
    if(b_indestructible_solid->IsPressed()) selected_solid_type = INDESTRUCTIBLE;

    if(b_save->IsPressed()){
        AddChild<FileDialogue>(FileDialogue::Modes::WRITE, Vector2f(0.0f,0.0f),Vector2f(680.0f,480.0f), "../res/map");
    }
}

void PingusWorldTourEditor::OnWidgetDraw(){

    //Move this to PingusWorldTour editor
    Vector2f mouse_position = Mouse::Get().GetPosition();

    float one_degree = 2.0f*ufoMaths::PI/360.0f;

    for(int a = 0; a < 360; a++){
        Graphics::Get().DrawLine(mouse_position + Vector2f(std::cos(a*one_degree),std::sin(a*one_degree))*brush_radius*level->GetActiveCamera()->scale, mouse_position + Vector2f(std::cos((a+1)*one_degree),std::sin((a+1)*one_degree))*brush_radius*level->GetActiveCamera()->scale,Graphics::WHITE);
    }

}