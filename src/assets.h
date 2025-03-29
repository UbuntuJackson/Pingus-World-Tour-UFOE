#include <asset_manager.h>
#include <ufo_engine.h>
    
///@assets;
void PingusWorldTourAssets(){

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
    
    Engine::Get().pixel_game_engine.LoadFontSprite("../res/assets/pwt_grey_font.png");

}
