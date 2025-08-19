#include <string>
#include <vector>
#include <memory>
#include <olcPixelGameEngine.h>
#include <sprite_reference.h>
#include <level.h>
#include <json_variant.h>
#include <ufo_maths.h>
#include <asset_manager.h>
#include <bitmap_collision_body.h>
#include "level_select_menu.h"
#include <widget.h>
#include <file_menu.h>
#include <single_keyboard.h>
#include <widget_sprite_reference.h>
//#include <sub_window.h>
#include "pingus_level.h"
#include "pingu.h"
#include <mouse.h>
#include <wrap_menu.h>
#include "level_select_menu.h"
#include "pingus_main_menu.h"
#include "generated.h"
#include "result_screen.h"
#include "pingu_selection_manager.h"
#include "level_decal_manager.h"

void PingusLevel::OnResourceLoad(){
    Engine::Get().wave_engine.StopAll();
    AssetManager::Get().current_music_track = "";
    AssetManager::Get().LoadDecal("../res/assets/loading.png","pwt_loading_screen");

    AssetManager::Get().LoadDecal("../res/assets/pingu_hitbox.png","pill");
    AssetManager::Get().LoadDecal("../res/assets/box_12_24.png","box_12_24");
    AssetManager::Get().LoadDecal("../res/assets/walking animation-Sheet.png", "pingu_walk");
    AssetManager::Get().LoadDecal("../res/assets/walking animation-SheetParachute.png", "pingu_walk_parachute");
    AssetManager::Get().LoadDecal("../res/assets/pingu_blocker.png", "pingu_blocker");
    AssetManager::Get().LoadDecal("../res/assets/fall_death-Sheet.png", "pingu_fall_death");
    AssetManager::Get().LoadDecal("../res/assets/spawner.png", "spawner");
    AssetManager::Get().LoadDecal("../res/assets/pingu_fall.png", "pingu_fall");
    AssetManager::Get().LoadDecal("../res/assets/pingu_parachute.png", "pingu_parachute");
    AssetManager::Get().LoadDecal("../res/assets/driller-Sheet.png", "pingu_driller");
    AssetManager::Get().LoadDecal("../res/assets/parachute.png", "parachute");
    AssetManager::Get().LoadDecal("../res/assets/honey_coint-Sheet.png", "honey_coin");
    AssetManager::Get().LoadDecal("../res/assets/pingu_blowing_up-Sheet.png", "pingu_explode");
    AssetManager::Get().LoadDecal("../res/assets/honey_coin_hud-Sheet.png", "honey_coin_hud");
    AssetManager::Get().LoadDecal("../res/assets/widget_theme.png", "pwt_widget_theme");
    AssetManager::Get().LoadDecal("../res/assets/widget_theme_grey.png", "pwt_widget_theme_grey");
    AssetManager::Get().LoadDecal("../res/assets/theme_grey_light.png", "pwt_theme_grey_light");
    AssetManager::Get().LoadDecal("../res/assets/theme_grey_dark.png", "pwt_theme_grey_dark");
    AssetManager::Get().LoadDecal("../res/assets/ranks.png", "ranks");
    AssetManager::Get().LoadDecal("../res/assets/pin.png","pin");
    AssetManager::Get().LoadDecal("../res/assets/red_x.png","red_x");
    AssetManager::Get().LoadDecal("../res/assets/backdrop.png","backdrop");
    AssetManager::Get().LoadDecal("../res/assets/pingus_splash_screen.png","pingus_splash_screen");
    AssetManager::Get().LoadDecal("../res/assets/pwt_intro_cutscene.png","pwt_intro_cutscene");
    AssetManager::Get().LoadDecal("../res/assets/pwt_intro_cutscene3.png","pwt_intro_cutscene3");
    AssetManager::Get().LoadDecal("../res/assets/cover_for_intro.png","cover_for_intro");
    AssetManager::Get().LoadDecal("../res/assets/pingu_build_icon.png","builder_icon");
    AssetManager::Get().LoadDecal("../res/assets/bomber_icon.png","bomber_icon");
    AssetManager::Get().LoadDecal("../res/assets/parachute_icon.png","parachute_icon");
    AssetManager::Get().LoadDecal("../res/assets/driller_icon.png","driller_icon");
    AssetManager::Get().LoadDecal("../res/assets/jumper_icon.png","jumper_icon");
    AssetManager::Get().LoadDecal("../res/assets/blocker_icon.png","blocker_icon");
    AssetManager::Get().LoadDecal("../res/assets/climber_icon.png","climber_icon");
    AssetManager::Get().LoadDecal("../res/assets/anti_matter_spawner.png","anti_matter_spawner");
    AssetManager::Get().LoadDecal("../res/assets/dash.png", "dash");
    
    Engine::Get().pixel_game_engine.LoadFontSprite("../res/assets/pwt_grey_font.png");

}

void PingusLevel::OnLoadActors(JsonDictionary& _actor_json){
    
    Generated::ActorJsonBridge(this, _actor_json);
}

void PingusLevel::OnLoad(JsonDictionary& _j){

    actor_sorting_method = actor_sorting_method_platformer;

    custom_level_size = asset_manager.GetDecal("solid")->sprite->Size();
    level_size = custom_level_size;
    Console::Out("Level Custom size =",level_size);
    GetActiveCamera()->world.x1 = custom_level_size.x;
    GetActiveCamera()->world.y1 = custom_level_size.y;
    GetActiveCamera()->clamp = true;
    Console::Out("Level size", level_size);

    NewActor<PinguSelectionManager>();
    NewActor<LevelDecalManager>();

    end_level_button = NewActor<Button>(Vector2f(180.0f,480.0f-25.0f), Vector2f(90.0f,32.0f), "Abort mission");

    rescued_pingus_label = NewActor<Label>(Vector2f(180.0f, 4.0f), Vector2f(100.0f, 10.0f), "Rescued Pingus");
    released_pingus_label = NewActor<Label>(Vector2f(175.0f, 21.0f), Vector2f(100.0f, 10.0f), "Released Pingus");
    max_pingus_label = NewActor<Label>(Vector2f(135.0f, 38.0f), Vector2f(100.0f, 10.0f), "Max. Rescuable Pingus");
    honey_coin_hud = NewActor<WidgetSpriteReference>("honey_coin_hud",
        Vector2f(Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x-33.0f, 52.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(32.0f, 32.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );
    honey_coin_hud->current_frame_index = 1.0f;
    end_level_button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    end_level_button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    end_level_button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
    end_level_button->visible = false;

    fast_forward_button = NewActor<Button>(Vector2f(180.0f,480.0f-25.0f-36.0f), Vector2f(90.0f,32.0f), "Fast forward >>");

    fast_forward_button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    fast_forward_button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    fast_forward_button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
    fast_forward_button->visible = false;

    pause_button = NewActor<Button>(Vector2f(180.0f,480.0f-25.0f-36.0f-36.0f), Vector2f(90.0f,32.0f), "Pause | |");

    pause_button->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    pause_button->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    pause_button->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);
    pause_button->visible = false;

    rescued_pingus_label->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    released_pingus_label->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    max_pingus_label->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    
}

void PingusLevel::OnCameraManipulation(){
    ControlActiveCameraWithMouse(Mouse::Get().GetMiddleButton().is_held, SingleKeyboard::Get().GetKey(olc::SHIFT).is_held);
    ControlActiveCameraWithKeyboard(
        SingleKeyboard::Get().GetKey(olc::D).is_held,
        SingleKeyboard::Get().GetKey(olc::A).is_held,
        SingleKeyboard::Get().GetKey(olc::W).is_held,
        SingleKeyboard::Get().GetKey(olc::S).is_held,
        160.0f);
}

void PingusLevel::OnUpdate(){
    
    Level::OnUpdate();
    pingu_selected_this_frame = false;

    if((end_level_button->IsReleased() || SingleKeyboard::Get().GetKey(olc::ESCAPE).is_pressed) && !is_menu){
        NewActor<ResultScreen>(Vector2f(30.0f,30.0f));
        level_finished = true;
        paused = true;

    }

    if(item_select_menu != nullptr){
        item_select_menu->ControlWithMouse();
        item_select_menu->ControlWithKeys(
            Mouse::Get().GetScrollDirection() > 0 && !SingleKeyboard::Get().GetKey(olc::SHIFT).is_held,
            Mouse::Get().GetScrollDirection() < 0 && !SingleKeyboard::Get().GetKey(olc::SHIFT).is_held,
            false
        );
    }

    if(paused){
        if(pause_button->IsReleased()){
            paused = false;
        }
        
        return;
    }

    if(is_menu){
        honey_coin_hud->visible = false;
        rescued_pingus_label->visible = false;
        released_pingus_label->visible = false;
        max_pingus_label->visible = false;
        return;
    }

    rescued_pingus_label->text = "Rescued Pingus:" + std::to_string(rescued_pingus) + "/" + std::to_string(required_pingus);
    released_pingus_label->text = "Released Pingus:" + std::to_string(released_pingus) + "/" + std::to_string(total_number_of_pingus);
    max_pingus_label->text = "Max. Rescuable Pingus:" + std::to_string(maximum_rescuable_pingus);

    end_level_button->local_position.x = Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x - 8.0f*end_level_button->text.size() + 10.0f;
    fast_forward_button->local_position.x = end_level_button->local_position.x;
    pause_button->local_position.x = end_level_button->local_position.x;
    
    rescued_pingus_label->local_position.x = Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x - 8.0f*rescued_pingus_label->text.size() - 12.0f;
    released_pingus_label->local_position.x = Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x - 8.0f*released_pingus_label->text.size() - 12.0f;
    max_pingus_label->local_position.x = Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x - 8.0f*max_pingus_label->text.size() -12.0f;
    
    rescued_pingus_label->rectangle.size.x = 8.0f*rescued_pingus_label->text.size()+8;
    released_pingus_label->rectangle.size.x = 8.0f*released_pingus_label->text.size()+8;
    max_pingus_label->rectangle.size.x = 8.0f*max_pingus_label->text.size()+8;

    //Console::Out("Are there pingus active?", at_least_one_pingu_active);

    if(!is_menu){
        end_level_button->visible = true;
        
        fast_forward_button->visible = true;
        fast_forward = false;
        if(fast_forward_button->IsHeld() || SingleKeyboard::Get().GetKey(olc::SPACE).is_held){
            fast_forward = true;
        }
        pause_button->visible = true;
        if(pause_button->IsReleased()){
            paused = true;
        }
    }

    if(!at_least_one_pingu_active && all_pingus_released){
        
        NewActor<ResultScreen>(Vector2f(30.0f,30.0f));
        level_finished = true;
        paused = true;
    }

    //This boolean is set after the if statement above, because the if statement above needs to be
    //evaluated after the last pingu has had their OnUpdate function run.
    all_pingus_released = released_pingus >= total_number_of_pingus;

    at_least_one_pingu_active = false;

    if(Mouse::Get().GetLeftButton().is_pressed){

        //auto a = NewActor<Pingu>(GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()));
    
    }
}

void PingusLevel::OnHandlePurge(){
    PurgeHandles(honey_coin_handles);
    PurgeHandles(world_map_location_handles);
    PurgeHandles(anti_matter_pingus);
    PurgeHandles(pingu_handles_all_pingus);
    //PurgeHandles(paintable_surface_handles);
}

void PingusLevel::OnExit(){
    Graphics::Get().DrawDecal(AssetManager::Get(),"pwt_loading_screen",Vector2f(0.0f,0.0f),Vector2f(0.0f,0.0f),Vector2f(0.0f,0.0f),Vector2f(680.0f,480.0f),Vector2f(1.0f,1.0f),0.0f, Graphics::WHITE);
}