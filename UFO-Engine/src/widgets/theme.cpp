#include <string>
#include "theme.h"
#include "../ufo_maths/ufo_maths.h"
#include "widget.h"
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcPixelGameEngine.h"
#include "../graphics/graphics.h"
#include "widget.h"

Theme::Theme(std::string _key, int _left_edge, int _right_edge, int _top_edge, int _bottom_edge) :
key{_key},
left_edge{_left_edge},
right_edge{_right_edge},
top_edge{_top_edge},
bottom_edge{_bottom_edge}
{
    non_default_theme = true;
}

void Theme::OnDraw(Widget* _widget){
    Vector2f decal_size = AssetManager::Get().GetDecal(key)->sprite->Size();

    //Top-Left Piece
    Graphics::Get().DrawDecal(key,
        _widget->GetGlobalPosition() + Vector2f(-left_edge, -top_edge),
        Vector2f(0.0f, 0.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(left_edge, top_edge),
        Vector2f(1.0f,1.0f),
        0.0f,
        tint
    );

    //Top-Middle Piece
    for(int i = 0; i < _widget->rectangle.size.x; i++){
        
        Graphics::Get().DrawDecal(key,
            _widget->GetGlobalPosition()+Vector2f(i, -top_edge),
            Vector2f(0.0f, 0.0f),
            Vector2f(left_edge, 0.0f),
            Vector2f(right_edge-left_edge, top_edge),
            Vector2f(1.0f,1.0f),
            0.0f,
            tint
        );
    }

    //Top-Right Piece
    Graphics::Get().DrawDecal(key,
        _widget->GetGlobalPosition() + Vector2f(_widget->rectangle.size.x, -top_edge),
        Vector2f(0.0f, 0.0f),
        Vector2f(right_edge, 0.0f),
        Vector2f(decal_size.x-right_edge, top_edge),
        Vector2f(1.0f,1.0f),
        0.0f,
        tint
    );

    //Left-Middle Piece
    for(int j = 0; j < _widget->rectangle.size.y; j++){
        
        Graphics::Get().DrawDecal(key,
            _widget->GetGlobalPosition()+Vector2f(-left_edge,j),
            Vector2f(0.0f, 0.0f),
            Vector2f(0.0f, top_edge),
            Vector2f(left_edge, bottom_edge-top_edge),
            Vector2f(1.0f,1.0f),
            0.0f,
            tint
        );
    }

    //Middle-Middle Piece
    
    Graphics::Get().DrawDecal(key,
        _widget->GetGlobalPosition(),
        Vector2f(0.0f, 0.0f),
        Vector2f(left_edge, top_edge),
        Vector2f(right_edge-left_edge, bottom_edge-top_edge),
        _widget->rectangle.size,
        0.0f,
        tint);

    //Right-Middle Piece
    for(int j = 0; j < _widget->rectangle.size.y; j++){
        
        Graphics::Get().DrawDecal(key,
            _widget->GetGlobalPosition()+Vector2f(_widget->rectangle.size.x, j),
            Vector2f(0.0f, 0.0f),
            Vector2f(right_edge, top_edge),
            Vector2f(decal_size.x - right_edge, bottom_edge-top_edge),
            Vector2f(1.0f,1.0f),
            0.0f,
            tint
        );
    }

    //Bottom-Left Piece
    Graphics::Get().DrawDecal(key,
        _widget->GetGlobalPosition() + Vector2f(-left_edge, _widget->rectangle.size.y),
        Vector2f(0.0f, 0.0f),
        Vector2f(0.0f, bottom_edge),
        Vector2f(left_edge, decal_size.y-bottom_edge),
        Vector2f(1.0f,1.0f),
        0.0f,
        tint
    );

    //Bottom-Middle Piece
    for(int i = 0; i < _widget->rectangle.size.x; i++){
        
        Graphics::Get().DrawDecal(key,
            _widget->GetGlobalPosition()+Vector2f(i, _widget->rectangle.size.y),
            Vector2f(0.0f, 0.0f),
            Vector2f(left_edge, bottom_edge),
            Vector2f(right_edge-left_edge, decal_size.y - bottom_edge),
            Vector2f(1.0f,1.0f),
            0.0f,
            tint
        );
    }

    //Bottom-Right Piece
    Graphics::Get().DrawDecal(key,
        _widget->GetGlobalPosition() + _widget->rectangle.size,
        Vector2f(0.0f, 0.0f),
        Vector2f(right_edge, bottom_edge),
        Vector2f(decal_size.x-right_edge, decal_size.y-bottom_edge),
        Vector2f(1.0f,1.0f),
        0.0f,
        tint
    );
}