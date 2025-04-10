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

PingusWorldTourEditor::PingusWorldTourEditor(Vector2f _) : Widget(Vector2f(0.0f,0.0f),Engine::Get().pixel_game_engine.GetWindowSizeInPixles()){}

void PingusWorldTourEditor::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level);

    b_save = AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(32.0f,0.0f),"Save");
    b_save->adjust_height_after_text_rows = true;
    b_load = AddChild<Button>(Vector2f(32.0f,0.0f),Vector2f(32.0f,0.0f),"Load");
    b_load->adjust_height_after_text_rows = true;
    b_import_layer = AddChild<Button>(Vector2f(32.0f*2,0.0f),Vector2f(56.0f,0.0f),"Import Layer");
    b_import_layer->adjust_height_after_text_rows = true;
    m_layers = AddChild<WrapMenu>(Vector2f(GetRectangle().size.x-200.0f,0.0f),Vector2f(200.0f,200.0f));
    AddLayer("solid", "solid.png");
    AddLayer("mg", "mg.png");
    
    level->is_menu = true;
}

void PingusWorldTourEditor::AddLayer(std::string _name, std::string _path){
    level->NewActor<PaintableSurface>(this,_name,_path);
    m_layers->AddChild<Button>(Vector2f(0.0f,0.0f),Vector2f(200.0f,32.0f),_name);
}

void PingusWorldTourEditor::OnUpdate(){
    int scroll_direction = Mouse::Get().GetScrollDirection();
    brush_radius += scroll_direction /** Engine::Get().GetDeltaTime()*/;
    if(brush_radius < 1.0f) brush_radius = 1.0f;
}

void PingusWorldTourEditor::OnWidgetDraw(){

    //Move this to PingusWorldTour editor
    Vector2f mouse_position = Mouse::Get().GetPosition();

    float one_degree = 2.0f*ufoMaths::PI/360.0f;

    for(int a = 0; a < 360; a++){
        Graphics::Get().DrawLine(mouse_position + Vector2f(std::cos(a*one_degree),std::sin(a*one_degree))*brush_radius*level->GetActiveCamera()->scale, mouse_position + Vector2f(std::cos((a+1)*one_degree),std::sin((a+1)*one_degree))*brush_radius*level->GetActiveCamera()->scale,Graphics::WHITE);
    }

}