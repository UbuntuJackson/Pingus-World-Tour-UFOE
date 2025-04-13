#pragma once
#include <asset_manager.h>
#include <widget_sprite_reference.h>
#include <level.h>
#include <olcPixelGameEngine.h>
#include <graphics.h>
#include <widget.h>
#include <ufo_maths.h>
#include <cmath>

class ColourPicker : public Widget{
public:
    const int red_hue = 255*255*2*1;
    const int blue_hue = 255*255*2*2;
    const int green_hue = 255*255*2*3;
    const int max_hue = (255+255)*3;
    const int min_hue = 0;
    int current_hue = 0;
    Colour hue = Colour(255,255,0);
    Vector2f current_colour_position;

    bool is_selecting_colour = false;
    bool is_selecting_hue = false;

    olc::Decal* colour_picker_decal = nullptr;
    olc::Decal* hue_slider_decal = nullptr;

    WidgetSpriteReference* colour_picker_sprite_ref = nullptr;
    WidgetSpriteReference* hue_slider_sprite_ref = nullptr;

    ColourPicker(Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){}

    Colour To255255255(float _r, float _g, float _b){
        return Colour(std::min(_r*255.0f,255.0f),std::min(_g*255.0f,255.0f),std::min(_b*255.0f,255.0f));
    }

    void OnLevelEnter(Level* _level){
        Widget::OnLevelEnter(_level);

        AssetManager::Get().sprites["colour_picker"] = std::make_unique<olc::Sprite>(GetRectangle().size.x, 200.0f);
        AssetManager::Get().decals["colour_picker"] = std::make_unique<olc::Decal>(AssetManager::Get().GetSprite("colour_picker"));
        colour_picker_decal = AssetManager::Get().GetDecal("colour_picker");
        colour_picker_sprite_ref = AddChild<WidgetSpriteReference>("colour_picker",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(GetRectangle().size.x, 200.0f),
            Vector2f(1.0f,1.0f),
            0.0f,0
        );

        for(int yy = 0; yy < GetRectangle().size.y; yy++){
            for(int xx = 0; xx < GetRectangle().size.x; xx++){
                float grey_ness = 1.0f-yy / GetRectangle().size.y;
                float saturation = 1.0f-xx / GetRectangle().size.x;
                float hue_r = 1.0f;
                float hue_g = 1.0f;
                float hue_b = 0.0f;
                colour_picker_decal->sprite->SetPixel(xx,yy,To255255255((hue_r+saturation)*grey_ness,(hue_g+saturation)*grey_ness,(hue_b+saturation)*grey_ness));
            }
        }
        colour_picker_decal->Update();

        AssetManager::Get().sprites["hue_slider"] = std::make_unique<olc::Sprite>(GetRectangle().size.x, 32.0f);
        AssetManager::Get().decals["hue_slider"] = std::make_unique<olc::Decal>(AssetManager::Get().GetSprite("hue_slider"));
        hue_slider_decal = AssetManager::Get().GetDecal("hue_slider");
        hue_slider_sprite_ref = AddChild<WidgetSpriteReference>("hue_slider",
            Vector2f(0.0f,200.0f),
            Vector2f(0.0f,0.0f),
            hue_slider_decal->sprite->Size(),
            Vector2f(1.0f,1.0f),
            0.0f,0
        );

        float f_red = 255.0f;
        float f_green = 0.0f;
        float f_blue = 0.0f;

        float colour_incrementation_unit = float(max_hue)/GetRectangle().size.x;
        
        for(int xx = 0; xx < GetRectangle().size.x; xx++){
            float s_hue = (xx / GetRectangle().size.x) * float(max_hue);
            Console::PrintLine("s_hue:",s_hue);
            
            if(s_hue >= 0.0f && s_hue < 255.0f){
                f_green += colour_incrementation_unit;
            }
            else if(s_hue >= 255.0f && s_hue < 255.0f*2.0f){
                
                f_red -= colour_incrementation_unit;
            }
            else if(s_hue >= 255.0f*2.0f && s_hue < 255.0f*3.0f){
                
                f_blue += colour_incrementation_unit;
            }
            else if(s_hue >= 255.0f*3.0f && s_hue < 255.0f*4.0f){
                
                f_green -= colour_incrementation_unit;
            }
            else if(s_hue >= 255.0f*4.0f && s_hue < 255.0f*5.0f){
                
                f_red += colour_incrementation_unit;
            }
            else if(s_hue >= 255.0f*5.0f && s_hue < 255.0f*6.0f){
                
                f_blue -= colour_incrementation_unit;
            }

            if(f_red < 0.0f) f_red = 0.0f;
            if(f_green < 0.0f) f_green = 0.0f;
            if(f_blue < 0.0f) f_blue = 0.0f;
            if(f_red > 255.0f) f_red = 255.0f;
            if(f_green > 255.0f) f_green = 255.0f;
            if(f_blue > 255.0f) f_blue = 255.0f;
            
            for(int yy = 0; yy < 32; yy++){
                hue_slider_decal->sprite->SetPixel(xx,yy,Colour(f_red, f_green, f_blue));
            }
        }
        
        hue_slider_decal->Update();
    }
    bool SetHue(){
        if(Mouse::Get().GetLeftButton().is_pressed && ufoMaths::RectangleVsPoint(ufo::Rectangle(hue_slider_sprite_ref->GetGlobalPosition(), hue_slider_sprite_ref->frame_size) , Mouse::Get().GetPosition())) is_selecting_hue = true;

        if(Mouse::Get().GetLeftButton().is_released) is_selecting_hue = false;

        if(Mouse::Get().GetLeftButton().is_held && ufoMaths::RectangleVsPoint(ufo::Rectangle(hue_slider_sprite_ref->GetGlobalPosition(), hue_slider_sprite_ref->frame_size) , Mouse::Get().GetPosition()) && is_selecting_hue){
            
            hue = hue_slider_decal->sprite->GetPixel(Mouse::Get().GetPosition()-hue_slider_sprite_ref->GetGlobalPosition());
            Console::PrintLine("Selected hue:",int(hue.r),int(hue.g),int(hue.b));
            UpdateColourGradient();
            return true;
        }
        return false;
    }

    void UpdateColourGradient(){
        float hue_r = (float)hue.r/255.0f;
        float hue_g = (float)hue.g/255.0f;
        float hue_b = (float)hue.b/255.0f;

        for(int yy = 0; yy < GetRectangle().size.y; yy++){
            for(int xx = 0; xx < GetRectangle().size.x; xx++){
                float grey_ness = 1.0f-yy / GetRectangle().size.y;
                float saturation = 1.0f-xx / GetRectangle().size.x;
                colour_picker_decal->sprite->SetPixel(xx,yy,To255255255((hue_r+saturation)*grey_ness,(hue_g+saturation)*grey_ness,(hue_b+saturation)*grey_ness));
            }
        }
        colour_picker_decal->Update();
    }

    bool SetColour(){
        if(Mouse::Get().GetLeftButton().is_pressed && ufoMaths::RectangleVsPoint(ufo::Rectangle(colour_picker_sprite_ref->GetGlobalPosition(), colour_picker_sprite_ref->frame_size) , Mouse::Get().GetPosition())) is_selecting_colour = true;

        if(Mouse::Get().GetLeftButton().is_released) is_selecting_colour = false;

        if(Mouse::Get().GetLeftButton().is_held && ufoMaths::RectangleVsPoint(ufo::Rectangle(colour_picker_sprite_ref->GetGlobalPosition(), colour_picker_sprite_ref->frame_size) , Mouse::Get().GetPosition()) && is_selecting_colour){
            current_colour_position = Mouse::Get().GetPosition()-colour_picker_sprite_ref->GetGlobalPosition();
            UpdateColourGradient();
            return true;
        }
        return false;
    }

    Colour GetColour(){
        return colour_picker_decal->sprite->GetPixel(current_colour_position);
    }
};