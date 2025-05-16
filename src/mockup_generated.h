#include <json_variant.h>
#include <ufo_maths.h>
#include <console.h>
#include <olcPixelGameEngine.h>
#include "../src/honey_coin.h"
#include "../src/pingu.h"
#include "../src/spawner.h"
#include "../src/goal.h"
#include "../src/pingus_main_menu.h"
#include "../src/item_select_menu.h"
#include "../src/test/static_quadtree_actor.h"
#include "../src/test/dynamic_quadtree_actor.h"
#include "../src/world_map_location.h"
#include "../src/test/movable_camera.h"
#include "../src/world_map_cursor.h"
#include "../src/world_map.h"
#include "../src/pingus_splash_screen.h"
#include "../src/story_mode_intro.h"
#include "../src/pingus_world_tour_editor.h"
#include "../src/moving_solid.h"

namespace Generated{

namespace nsMovingSolid{

    //Setting tileset_name already in project.py?
    const std::string category = "moving_solid";

    olc::Decal* GetIcon(Level* _level, MovingSolid* _actor){
        ufo::Rectangle rect = AssetManager::Get().GetFrameFromSpriteSheet(_actor->spr->key, _actor->moving_solid_id, Vector2f(100.0f,100.0f));
        
        std::unique_ptr<olc::Sprite> spr = std::make_unique<olc::Sprite>(rect.size);
        for(int yy = rect.position.y; yy < rect.size.y; yy++){
            for(int xx = rect.position.x; xx < rect.size.x; xx++){
                spr->SetPixel(Vector2i(xx-int(rect.position.x),yy-int(rect.position.y)),_level->asset_manager.GetDecal(_actor->spr->key)->sprite->GetPixel(xx,yy));
            }
        }

        std::string name_plus_id = std::to_string(_actor->moving_solid_id);
        _level->asset_manager.sprites[name_plus_id] = std::move(spr);
        _level->asset_manager.decals[name_plus_id] = std::make_unique<olc::Decal>(_level->asset_manager.sprites[name_plus_id].get());

        return _level->asset_manager.GetDecal(name_plus_id);
    }

    void ImportAdditionalEditorAttributes(MovingSolid* _actor){
        //_actor->category = category;
    }
};

}