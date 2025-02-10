#ifndef SPRITE_COLLISION_H
#define SPRITE_COLLISION_H

#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"

bool
CollisionBody::IsOverlappingOtherDecal(olc::Decal *_decal, olc::vf2d _position, olc::Decal *_other_decal, olc::vf2d _other_position, olc::Pixel _colour){

    for(int y = int(_position.y); y < int(_position.y) + _decal->sprite->Size().y; y++){
        for(int x = int(_position.x); x < int(_position.x) + _decal->sprite->Size().x; x++){
            if(CompareColour(_decal->sprite->GetPixel(x- int(_position.x), y- int(_position.y)), olc::WHITE)
                && CompareColour(_other_decal->sprite->GetPixel(x - int(_other_position.x), y - int(_other_position.y)), _colour)
            ){
                return true;
            }
        }
    }
    return false;
}

/*
bool
CollisionBody::IsOverlappingSolid_Or_SemiSolid(Level* _map, olc::Decal *_decal, std::string _layer, olc::vf2d _position){
    for(auto [k, v] : semisolid_colours_overlapped){
        olc::Pixel colour = StringToColour(k);
        if(!IsOverlapping(_map, mask_decal, solid_layer, _position, colour) &&
            !v &&
            velocity.y > 0.0f
        ){
            return true;
        }
    }
    if(IsOverlapping(_map, mask_decal, solid_layer, _position, olc::WHITE)) return true;
    return false;
}

//should be IsOverlapping(olc::Decal *_decal, olc::vf2d _position, std::string _other_decal, olc::vf2d _other_position, olc::Pixel _colour)
//can make a function that iterates through actor layers and makes use of this function

void
CollisionBody::AdjustEnterDynamicSolidX(LayerActor* _act_layer){
    for(auto act : _act_layer->actors){
        olc::vf2d act_new_position = act->position;
        act_new_position.x = act->position.x + act->velocity.x;

        if(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
            int a = 0;
            if(velocity.x > 0.0f){ //next frame
                position.x = std::floor(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.x -= 1.0f;
                }
            }
            if(velocity.x < 0.0f){
                position.x = std::ceil(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.x += 1.0f;
                }
            }
            velocity.x = 0.0f;
        }
    }
}

void
CollisionBody::AdjustEnterDynamicSolidY(LayerActor* _act_layer){
    for(auto act : _act_layer->actors){
        olc::vf2d act_new_position = act->position + act->velocity; //Maybe check for a different position

        if(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
            if(velocity.y > 0.0f){
                position.y = std::floor(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.y -= 1.0f;
                }
            }
            if(velocity.y < 0.0f){
                position.y = std::ceil(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.y += 1.0f;
                }
            }
            velocity.y = 0.0f;
        }
    }
}

void
CollisionBody::AdjustEnterPseudoStaticSolidX(LayerActor* _act_layer){
    //We compare with the future state of DynamicSolid here to make sure we are out of collision by the end of the frame,
    //because if Player is relative to Dynamic solid, it is reasonable to emulate a scenario where both the x and y velocity is added
    //the behaviour should emulate that of a normal solid.
    for(auto act : _act_layer->actors){
        olc::vf2d act_new_position = act->position + act->velocity;

        if(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
            std::cout << "help" << std::endl;
            int a = 0;
            if(velocity.x > 0.0f){ //next frame
                position.x = std::floor(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.x -= 1.0f;
                }
            }
            if(velocity.x < 0.0f){
                position.x = std::ceil(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.x += 1.0f;
                }
            }
            velocity.x = 0.0f;
        }
    }
}

void
CollisionBody::AdjustEnterPseudoStaticSolidY(LayerActor* _act_layer){
    //We compare with the future state of DynamicSolid here to make sure we are out of collision by the end of the frame,
    //because if Player is relative to Dynamic solid, it is reasonable to emulate a scenario where both the x and y velocity is added
    //the behaviour should emulate that of a normal solid.
    for(auto act : _act_layer->actors){
        olc::vf2d act_new_position = act->position + act->velocity;
        if(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
            if(velocity.y > 0.0f){
                position.y = std::floor(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.y -= 1.0f;
                }
            }
            if(velocity.y < 0.0f){
                position.y = std::ceil(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.y += 1.0f;
                }
            }
            velocity.y = 0.0f;
        }
    }
}

void
CollisionBody::AdjustEnteredDynamicSolidX(LayerActor* _act_layer){
    for(auto act : _act_layer->actors){
        olc::vf2d act_new_position = act->position;
        act_new_position.x = act->position.x + act->velocity.x; //Dynamic solid moves in along the X-axis

        if(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
            int a = 0;
            if(act->velocity.x < 0.0f){ //next frame
                position.x = std::floor(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.x -= 1.0f;
                }
            }
            if(act->velocity.x > 0.0f){ //next frame
                position.x = std::ceil(position.x);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.x += 1.0f;
                }
            }
            velocity.x = 0.0f;
        }
    }
}

void
CollisionBody::AdjustEnteredDynamicSolidY(LayerActor* _act_layer){
    for(auto act : _act_layer->actors){
        olc::vf2d act_new_position = act->position + act->velocity; //Dynamic solid moves in along the X-axis

        if(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
            if(act->velocity.y < 0.0f){
                position.y = std::ceil(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.y -= 1.0f;
                }
            }
            if(act->velocity.y > 0.0f){
                position.y = std::floor(position.y);
                while(IsOverlappingOtherDecal(mask_decal, position, game->asset_manager.GetDecal(act->mask), act_new_position)){
                    position.y += 1.0f;
                }
            }
            velocity.y = 0.0f;
        }
    }
}
*/

void
AdjustCollisionX(DynamicShape<ufoSprite>& _actor_shape_sprite, std::string& _level_collision_decal_key){ //What information should be passed in?
    auto level_collision_decal = AssetManager::Get().GetDecal(_level_collision_decal_key);
    auto mask_decal = AssetManager::Get().GetDecal(_actor_shape_sprite.shape.key);
    
    if(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
        if(_actor_shape_sprite.velocity.x > 0.0f){
            _actor_shape_sprite.shape.position.x = std::floor(_actor_shape_sprite.shape.position.x); //Do I need floor and ceil?
            while(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
                _actor_shape_sprite.shape.position.x -= 1.0f;
            }
        }
        if(_actor_shape_sprite.velocity.x < 0.0f){
            _actor_shape_sprite.shape.position.x = std::ceil(_actor_shape_sprite.shape.position.x);
            while(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
                _actor_shape_sprite.shape.position.x += 1.0f;
            }
        }
        _actor_shape_sprite.velocity.x = 0.0f;
    }
}

void
AdjustCollisionY(DynamicShape<ufoSprite>& _actor_shape_sprite, std::string& _level_collision_decal_key){ //What information should be passed in?
    auto level_collision_decal = AssetManager::Get().GetDecal(_level_collision_decal_key);
    auto mask_decal = AssetManager::Get().GetDecal(_actor_shape_sprite.shape.key);
    
    if(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
        if(_actor_shape_sprite.velocity.y > 0.0f){
            _actor_shape_sprite.shape.position.y = std::floor(_actor_shape_sprite.shape.position.y); //Do I need floor and ceil?
            while(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
                _actor_shape_sprite.shape.position.y -= 1.0f;
            }
        }
        if(_actor_shape_sprite.velocity.y < 0.0f){
            _actor_shape_sprite.shape.position.y = std::ceil(_actor_shape_sprite.shape.position.y);
            while(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
                _actor_shape_sprite.shape.position.y += 1.0f;
            }
        }
        _actor_shape_sprite.velocity.y = 0.0f;
    }
}

void
CB_ApplyUpSlope(DynamicShape<ufoSprite>& _actor_shape_sprite, std::string& _level_collision_decal_key){
    auto level_collision_decal = AssetManager::Get().GetDecal(_level_collision_decal_key);
    auto mask_decal = AssetManager::Get().GetDecal(_actor_shape_sprite.shape.key);

    int up_slope_range = 10;

    if(IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
        _actor_shape_sprite.shape.position = former_position;
        float direction = 0.0f;
        if(_actor_shape_sprite.velocity.x > 0.0f) direction = 1.0f;
        if(_actor_shape_sprite.velocity.x < 0.0f) direction = -1.0f;

        //This loop is to iterate from start position to end position. The +1 is to make sure the distance isn't somehow rounded down.
        for(int x_step = 0; x_step < (int)(std::abs(velocity.x*direction)+1); x_step++){
            olc::vf2d before_height_adj = _actor_shape_sprite.shape.position;        
            _actor_shape_sprite.shape.position.x += direction;
            for(int step = 0; step < up_slope_range; step++){
                _actor_shape_sprite.shape.position.y -= 1.0f;
                
                //If able to move full range
                if(
                    !IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f}) &&
                    x_step == (int)(std::abs(velocity.x*direction)) &&
                    step == up_slope_range-1
                ){
                    _actor_shape_sprite.shape.position.x += _actor_shape_sprite.velocity.x;
                    break;
                }

                //Unsure if this is needed but it seems to work.
                if(!IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})) break;
                if(up_slope_range-1 == step && IsOverlapping(_lvl, mask_decal, solid_layer, _actor_shape_sprite.shape.position)){
                    _actor_shape_sprite.shape.position = before_height_adj;
                }
            }
        }
    }
}

/*void
CellActor::ApplyDownSlope_SemiSolid(CellMap* _map){
    int distance_under_character = 0;
    while(!IsOverlapping(_map, mask_decal, solid_layer, {int(position.x), int(position.y) + distance_under_character}, colour)){
        for(auto [k, v] : semisolid_colours_overlapped){
            olc::Pixel colour = StringToColour(k);
            if(!IsOverlapping(_map, mask_decal, solid_layer, {int(position.x), int(position.y) + distance_under_character}, colour) &&
                !v &&
                velocity.y > 0.0f
            ){
                
                distance_under_character++;
                
            }
            else{
                distance_under_character--;
            }
        }
    }
}*/

void
CB_AdjustDownSlope(DynamicShape<ufoSprite>& _actor_shape_sprite, std::string& _level_collision_decal_key){
    auto level_collision_decal = AssetManager::Get().GetDecal(_level_collision_decal_key);
    auto mask_decal = AssetManager::Get().GetDecal(_actor_shape_sprite.shape.key);

    if(!(was_grounded == true && is_grounded == false && _actor_shape_sprite.velocity.y > 0.0f)) return;

    int down_slope_range = 8;
    if(!IsOverlappingOtherDecal(mask_decal, olc::vf2d(_actor_shape_sprite.shape.position.x, _actor_shape_sprite.shape.position.y + 1.0f),level_collision_decal, {0.0f, 0.0f}, olc::WHITE)){
        for(int i= 0; i < down_slope_range; i++){
            olc::vf2d before_down_slope_adj = position;
            if(!IsOverlappingOtherDecal(mask_decal, olc::vf2d(_actor_shape_sprite.shape.position.x, _actor_shape_sprite.shape.position.y + 1.0f), level_collision_decal,{0.0f, 0.0f}, olc::WHITE)){
                _actor_shape_sprite.shape.position.y += 1.0f;
            }
            else{
                break;
            }
            if(i == down_slope_range-1 && IsOverlappingOtherDecal(mask_decal, _actor_shape_sprite.shape.position, level_collision_decal, {0.0f, 0.0f})){
                _actor_shape_sprite.shape.position = before_down_slope_adj;
            }
        }
    }
}

/*
olc::Pixel StringToColour(std::string _colour_name){
    if(_colour_name == "red") return olc::RED;
    if(_colour_name == "orange") return UfoGlobal::ORANGE;
    if(_colour_name == "yellow") return UfoGlobal::YELLOW;
    if(_colour_name == "lemon") return UfoGlobal::LEMON;
}

bool IsAlreadyInSolid(std::string _colour_name){
    return semisolid_colours_overlapped.at(_colour_name);
}

void
UpdateSemiSolidOverlapStatus(Level* _map){
    for(auto [k,v] : semisolid_colours_overlapped){
        semisolid_colours_overlapped[k] = IsOverlapping(_map, mask_decal, solid_layer, position, StringToColour(k));
    }
}*/

void
ApplyCollisionNaive(const ufoSprite& _actor_shape_sprite, const std::string& _level_collision_decal_key){
    auto level_collision_decal = AssetManager::Get().GetDecal(_level_collision_decal_key);
    auto actor_shape_decal = AssetManager::Get().GetDecal(_actor_shape_sprite.shape.key);

    former_position = _actor_shape_sprite.shape.position;
    _actor_shape_sprite.shape.position.x += _actor_shape_sprite.velocity.x;
    ApplyUpSlope_SemiSolid(_actor_shape_sprite, _level_collision_decal_key);
    CB_ApplyUpSlope(_actor_shape_sprite, _level_collision_decal_key);
    AdjustCollisionX(_actor_shape_sprite, _level_collision_decal_key);
    //UpdateSemiSolidOverlapStatus(_actor_shape_sprite, _level_collision_decal_key);
    _actor_shape_sprite.shape.position.y += velocity.y;
    AdjustCollisionY(_actor_shape_sprite, _level_collision_decal_key);
    /*for(auto [k, v] : semisolid_colours_overlapped){
        if(IsOverlapping(_map,mask_decal, solid_layer, position, StringToColour(k)) &&
            velocity.y > 0.0f &&
            !v){
            
            position.y = std::floor(position.y);
            while(IsOverlapping(_map,mask_decal, solid_layer, position, StringToColour(k))){
                position.y -= 1.0f;
            }

            velocity.y = 0.0f;
        }
    }*/
    was_grounded = is_grounded;
    is_grounded = false;

    /*for(auto [k, v] : semisolid_colours_overlapped){
        if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}, StringToColour(k))){
            is_grounded = true;        
        }
    }*/

    if(IsOverlappingOtherDecal(mask_decal,{position.x,position.y+1.0f}, level_collision_decal, {0.0f,0.0f})){
        is_grounded = true;        
    }

    CB_AdjustDownSlope(_actor_shape_sprite, _level_collision_decal_key);

    /*for(auto [k, v] : semisolid_colours_overlapped){
        if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f}, StringToColour(k))){
            is_grounded = true;        
        }
    }
    if(IsOverlapping(_map,mask_decal,solid_layer,{position.x,position.y+1.0f})){
        is_grounded = true;        
    }*/

}

#endif