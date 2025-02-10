#include "../ufo_maths/ufo_maths.h"
#include "floating_block.h"
#include "tuxtale_player.h"
#include "tuxtale_level.h"
#include "info_block.h"
#include "../sprite_reference/sprite_reference.h"
#include "coin_effect.h"
#include "../asset_manager/asset_manager.h"
#include "../widgets/one_dimensional_text_box.h"
#include "../keyboard/single_keyboard.h"

InfoBlock::InfoBlock(Vector2f _local_position) : FloatingBlock(_local_position){

}

void
InfoBlock::OnLevelEnter(Level* _level){
    FloatingBlock::OnLevelEnter(_level);
    Console::Out("InfoBlock");

    sprite_ref = AddChild<SpriteReference>(
        "info_block_2p5d",
        Vector2f(0.0f, -24.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(16.0f, 24.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );
}

void
InfoBlock::OnUpdate(){
    FloatingBlock::OnUpdate();
    if(tb){
        if(SingleKeyboard::Get().GetKey(olc::N).is_pressed){
            bool has_next = tb->Next();
            if(!has_next){
                tb = nullptr;
            }
        }
        if(SingleKeyboard::Get().GetKey(olc::B).is_pressed){
            bool has_previous = tb->Back();
        }
    }
}

void
InfoBlock::OnHit(TuxTalePlayer* _player){
    if(tb) return;

    auto u_text_box = std::make_unique<OneDimensionalTextBox>(Vector2f(20.0f,240.0f-20.0f-50.0f), Vector2f(340.0-40.0f, 53.0f)); 

    u_text_box->text_queue.push_back(text);

    std::reverse(u_text_box->text_queue.begin(), u_text_box->text_queue.end());

    tb = u_text_box.get();

    level->NewActor(std::move(u_text_box));
}
void
InfoBlock::OnOutOfRange(TuxTalePlayer* _player){
    if(tb){
        tb->QueueForPurge();
        tb = nullptr;
    }
}