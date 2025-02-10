
#include <memory>
#include "tuxtale_player.h"
#include "tuxtale_level.h"
#include "../forward/animation_forward.h"
#include "../console/console.h"
#include "../json/json.h"
#include "grass_crawler.h"
#include "../level/ufo_engine.h"
#include "enemy.h"
#include "../level/level_transition_effect.h"

using namespace ufoMaths;

GrassCrawler::GrassCrawler(Vector2f _position) : Enemy(_position,Rectangle(Vector2f(1.0f,1.0f), Vector2f(14.0f,14.0f))){

}

void GrassCrawler::OnLevelEnter(Level* _level){
    Enemy::OnLevelEnter(_level);
    auto u_grass_crawler_anim = std::make_unique<Animation>(Vector2f(8.0f, 4.0f));

    u_grass_crawler_anim->AddAnimationState(AnimatedSpriteReference(
        "new_grass_crawler",
        Vector2f(0.0f, 0.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(32.0f, 32.0f),
        Vector2f(1.0f, 1.0f),
        0.0f,
        0,
        8.01f
    ));

    u_grass_crawler_anim->SetAnimation("new_grass_crawler");

    animation = u_grass_crawler_anim.get();

    AddChild(std::move(u_grass_crawler_anim));

    AddChild(std::make_unique<SpriteReference>(
        "highlight",
        Vector2f(8.0f, 4.0f),
        Vector2f(16.0f, 16.0f),
        Vector2f(32.0f, 32.0f),
        Vector2f(1.0f, 1.0f),
        0.0f,
        0
    ));

    level = dynamic_cast<TuxTaleLevel*>(_level);
}

bool GrassCrawler::FoundPath(const Rectangle& _rectangle){
    int number_of_tiles = 0;

    for(int yy = (int)_rectangle.position.y/16 - (2); yy <= (int)_rectangle.position.y/16 + (2); yy++){
        for(int xx = (int)_rectangle.position.x/16 - (2); xx <= (int)_rectangle.position.x/16 + (2); xx++){
            TilesetData data = level->tilemap.GetTilesetData("collision_tiles");

            int tile_id = level->tilemap.tilemap_collision_data[xx+yy*level->tilemap.number_of_columns] - data.tileset_start_id + 1;

            switch(tile_id){
                case 11:{
                    if(ufoMaths::RectangleVsRectangle(_rectangle, Rectangle(Vector2f(xx*16.0f,yy*16.0f), Vector2f(16.0f,16.0f)))){
                        return true;
                    }
                }
                break;
            }

            number_of_tiles++;
        }
    }

    return false;
}

void GrassCrawler::OnUpdate(){
    //if(!ufoMaths::RectangleVsRectangle(level->GetActiveCamera()->GetOnScreenRectangleInWorld(),shape)) return;

    Shape<Rectangle>::OnUpdate();

    Vector2f temp_rect_position = local_position+Vector2f(1.0f, 1.0f);

    if(!FoundPath(Rectangle(temp_rect_position+direction, Vector2f(14.0f,14.0f)))){
        if(FoundPath(Rectangle(temp_rect_position+Ray2(temp_rect_position, temp_rect_position+direction).Normal(), Vector2f(14.0f,14.0f)))){
            direction = Ray2(temp_rect_position, temp_rect_position+direction).Normal();
        }
        if(FoundPath(Rectangle(temp_rect_position-Ray2(temp_rect_position, temp_rect_position+direction).Normal(), Vector2f(14.0f,14.0f)))){
            direction = -Ray2(temp_rect_position, temp_rect_position+direction).Normal();
        }
        
    }

    local_position += direction*Engine::Get().GetDeltaTime();

    
    animation->current_animation_state->rotation = atan2(-direction.y, -direction.x);

    if(direction.x > 0.0f) animation->current_animation_state->scale.y = -1.0f;
    if(direction.x < 0.0f) animation->current_animation_state->scale.y = 1.0f;
    
}

void GrassCrawler::OnHit(TuxTalePlayer* _player){
    level->NewActor(std::make_unique<LevelTransitionEffect>(battle_arena_path));
}

std::unique_ptr<GrassCrawler> GrassCrawler::Load(Json& _json){
    return std::make_unique<GrassCrawler>(Vector2f((float)_json.GetAsInt("x"), (float)_json.GetAsInt("y")));
}