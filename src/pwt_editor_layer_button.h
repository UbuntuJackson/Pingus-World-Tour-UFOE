#pragma once

#include <button.h>

class PingusWorldTourEditor;
class PaintableSurface;

class PWTEditorLayerButton : public Button{
public:
    PingusWorldTourEditor* editor = nullptr;
    PaintableSurface* layer = nullptr;
    PWTEditorLayerButton(PingusWorldTourEditor* _editor, PaintableSurface* _layer,Vector2f _local_position, Vector2f _size, std::string _text);

    void OnUpdate();
};