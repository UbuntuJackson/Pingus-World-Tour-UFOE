#include <button.h>
#include <console.h>
#include "pwt_editor_layer_button.h"
#include "pingus_world_tour_editor.h"
#include "paintable_surface.h"

PWTEditorLayerButton::PWTEditorLayerButton(PingusWorldTourEditor* _editor, PaintableSurface* _layer,Vector2f _local_position, Vector2f _size, std::string _text) :
Button(_local_position, _size, _text),
editor{_editor},
layer{_layer}{
    
}

void PWTEditorLayerButton::OnStart(Level* _level){
    SelectLayer();
    Console::PrintLine("PWTEditorLayerButton::OnStart",editor->selected_layer->visual_suface_name);
}

void PWTEditorLayerButton::SelectLayer(){
    if(editor->selected_layer != nullptr){
        if(editor->selected_layer->visual_suface_name == "solid") editor->selected_layer->visual_surface_ref->tint.a = 0;
    }
    editor->selected_layer = layer;
    is_selected = true;
    if(editor->selected_layer->visual_suface_name == "solid"){
        editor->selected_layer->visual_surface_ref->tint.a = 255;
    }
}

void PWTEditorLayerButton::OnUpdate(){
    Button::OnUpdate();

    if(IsPressed()){
        SelectLayer();
    }
}