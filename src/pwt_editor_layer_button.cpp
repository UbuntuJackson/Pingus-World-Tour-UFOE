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

void PWTEditorLayerButton::OnUpdate(){
    Button::OnUpdate();

    if(is_selected){
        //Console::PrintLine("PWTEditorLayerButton:", layer->visual_suface_name);
        editor->selected_layer = layer;
    }
}