#ifndef TUXTALE_HUD_H
#define TUXTALE_HUD_H
#include <vector>
#include "../sprite_reference/widget_sprite_reference.h"
#include "../widgets/widget.h"
#include "../level/ufo_engine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../keyboard/single_keyboard.h"
#include "../widgets/wrap_menu.h"
#include "battle_player.h"
#include "battle_enemy.h"
#include "../widgets/one_dimensional_text_box.h"
#include "battle_participant.h"

using namespace ufoMaths;

class TuxTaleHud : public Widget{
public:
    bool buttons_added = false;
    int selected_active_action = 0;
    int state = 0;

    std::vector<BattleParticipant*> battle_participant_handles;
    int turn = 0;

    bool should_exit_battle = false;

    std::vector<WrapMenu*> wrap_menu_stack;
    BattlePlayer* battle_player = nullptr;

    OneDimensionalTextBox* text_box = nullptr;

    std::vector<WidgetSpriteReference*> action_button_handles;

    enum BattleModeStates{
        ACTION_SELECT,
        ITEM_SELECT,
        CAN_MOVE,
        DIALOGUE,
        OPPONENT_TURNS
    };

    TuxTaleHud() : Widget(Vector2f(0.0f, 0.0f),Vector2f(340.0f,240.0f)){
        state = BattleModeStates::ACTION_SELECT;
    }

    void OnLevelEnter(Level* _level){
        Widget::OnLevelEnter(_level);

        float button_width = 41.0f;

        Vector2f button_bar_start_position = Vector2f(45.0f,240-40.0f);

        AddChild(std::make_unique<WidgetSpriteReference>(
            "attack",
            button_bar_start_position+Vector2f(0.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(41.0f, 37.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        ));
        AddChild(std::make_unique<WidgetSpriteReference>(
            "act",
            button_bar_start_position+Vector2f(button_width*1.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(41.0f, 37.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        ));
        AddChild(std::make_unique<WidgetSpriteReference>(
            "magic",
            button_bar_start_position+Vector2f(button_width*2.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(41.0f, 37.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        ));
        AddChild(std::make_unique<WidgetSpriteReference>(
            "item",
            button_bar_start_position+Vector2f(button_width*3.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(41.0f, 37.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        ));
        AddChild(std::make_unique<WidgetSpriteReference>(
            "defend",
            button_bar_start_position+Vector2f(button_width*4.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(41.0f, 37.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        ));
        AddChild(std::make_unique<WidgetSpriteReference>(
            "end",
            button_bar_start_position+Vector2f(button_width*5.0f, 0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(41.0f, 37.0f),
            Vector2f(1.0f, 1.0f),
            0.0f,
            0
        ));

        auto u_battle_player = std::make_unique<BattlePlayer>(Vector2f(110.0f, 150.0f), this);

        u_battle_player->SetZIndex(3);
        _level->should_resort_after_z_index = true;

        battle_player = u_battle_player.get();

        _level->NewActor(std::move(u_battle_player));


        auto u_battle_enemy = std::make_unique<BattleEnemy>(Vector2f(220.0f, 160.0f),this);
        u_battle_enemy->SetZIndex(2);
        _level->NewActor(std::move(u_battle_enemy));

        std::unique_ptr<OneDimensionalTextBox> u_tb = std::make_unique<OneDimensionalTextBox>(Vector2f(50.0f, 50.0f), Vector2f(200.0f, 100.0f));

        u_tb->text_queue.push_back("Use the action select menu to take immediate action!");
        u_tb->text_queue.push_back("...");
        u_tb->text_queue.push_back("The grass crawler gives you a bitter stare");
        u_tb->text_queue.push_back("You stumbled upon a grass-crawler!");

        text_box = u_tb.get();

        state = BattleModeStates::DIALOGUE;

        AddChild(std::move(u_tb));
        
    }

    void OnUpdate(){
        if(!buttons_added){
            for(const auto& button : children){
                if(button->GetType() == "WidgetSpriteReference"){
                    WidgetSpriteReference* spr_button = dynamic_cast<WidgetSpriteReference*>(button.get());
                    spr_button->current_frame_index = 1;
                    action_button_handles.push_back(spr_button);
                }
            }
            buttons_added = true;

            action_button_handles[selected_active_action]->current_frame_index = 0;
            action_button_handles[selected_active_action]->local_position.y-=6.0f;
        }

        if(state == BattleModeStates::ACTION_SELECT){
            if(wrap_menu_stack.size() == 0){

                if(SingleKeyboard::Get().GetKey(olc::LEFT).is_pressed){
                    action_button_handles[selected_active_action]->current_frame_index = 1;
                    action_button_handles[selected_active_action]->local_position.y+=6.0f;
                    selected_active_action--;
                    selected_active_action = ufoMaths::Wrap(selected_active_action, 0, int(action_button_handles.size()));
                    action_button_handles[selected_active_action]->current_frame_index = 0;
                    action_button_handles[selected_active_action]->local_position.y-=6.0f;
                }
                if(SingleKeyboard::Get().GetKey(olc::RIGHT).is_pressed){
                    action_button_handles[selected_active_action]->current_frame_index = 1;
                    action_button_handles[selected_active_action]->local_position.y+=6.0f;
                    selected_active_action++;
                    selected_active_action = ufoMaths::Wrap(selected_active_action, 0, int(action_button_handles.size()));
                    action_button_handles[selected_active_action]->current_frame_index = 0;
                    action_button_handles[selected_active_action]->local_position.y-=6.0f;
                }

                if(SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed){
                    auto u_wrap_menu = std::make_unique<WrapMenu>(Vector2f(50.0f, 50.0f), Vector2f(100.0f, 100.0f));
                    switch(selected_active_action){
                        case 0:
                        {
                            auto u_button_jump = std::make_unique<Button>(Vector2f(0.0f, 0.0f), Vector2f(100.0f, 20.0f), "Slap");
                            u_button_jump->on_pressed = [&](Widget* _w, Button* _this_button){
                                dynamic_cast<TuxTaleHud*>(_w->parent)->state = BattleModeStates::CAN_MOVE;
                                _w->QueueForPurge();
                            };
                            auto u_button_use_politics = std::make_unique<Button>(Vector2f(0.0f, 0.0f), Vector2f(100.0f, 20.0f), "...");

                            u_wrap_menu->AddChild(std::move(u_button_jump));
                            u_wrap_menu->AddChild(std::move(u_button_use_politics));


                            wrap_menu_stack.push_back(u_wrap_menu.get());
                            AddChild(std::move(u_wrap_menu));
                        }
                        break;
                        case 1:
                        {
                            auto u_button_jump = std::make_unique<Button>(Vector2f(0.0f, 0.0f), Vector2f(100.0f, 20.0f), "Let's just talk it out...");
                            u_button_jump->on_pressed = [&](Widget* _w, Button* _this_button){
                                dynamic_cast<TuxTaleHud*>(_w->parent)->state = BattleModeStates::CAN_MOVE;
                                _w->QueueForPurge();

                                std::unique_ptr<OneDimensionalTextBox> u_tb = std::make_unique<OneDimensionalTextBox>(Vector2f(50.0f, 50.0f), Vector2f(100.0f, 100.0f));

                                u_tb->text_queue.push_back("You tried sorting it out with the enemy but your motives are vastly different.");

                                text_box = u_tb.get();

                                state = BattleModeStates::DIALOGUE;

                                dynamic_cast<TuxTaleHud*>(_w->parent)->AddChild(std::move(u_tb));
                                
                            };

                            u_wrap_menu->AddChild(std::move(u_button_jump));

                            wrap_menu_stack.push_back(u_wrap_menu.get());
                            AddChild(std::move(u_wrap_menu));
                        }
                        break;
                        case 5:
                        {

                            std::unique_ptr<OneDimensionalTextBox> u_tb = std::make_unique<OneDimensionalTextBox>(Vector2f(50.0f, 50.0f), Vector2f(100.0f, 100.0f));

                            u_tb->text_queue.push_back("For debugging purposes you can always exit the battle :)");

                            text_box = u_tb.get();

                            state = BattleModeStates::DIALOGUE;

                            AddChild(std::move(u_tb));

                            should_exit_battle = true;
                        }
                        break;
                    }

                }
            }
            else{
                if(SingleKeyboard::Get().GetKey(olc::ENTER).is_pressed){
                    wrap_menu_stack.back()->QueueForPurge();
                    wrap_menu_stack.pop_back();
                }
            }
        }

        if(state == BattleModeStates::DIALOGUE){
            if(text_box){
                if(SingleKeyboard::Get().GetKey(olc::Z).is_pressed){
                    bool has_next = text_box->Next();
                    if(!has_next){
                        text_box = nullptr;
                        state = BattleModeStates::ACTION_SELECT;
                        wrap_menu_stack.clear();

                        if(should_exit_battle){
                            Engine::Get().GetActiveProfile()->Load();
                        }
                    }
                }

                /*if(SingleKeyboard::Get().GetKey(olc::B).is_pressed){
                    bool has_previous = text_box->Back();
                }*/
            }
        }

        
        battle_player->can_move = (state == BattleModeStates::CAN_MOVE);

        
        BattleParticipant* current_participant = GetCurrentOpponent();
        if(current_participant){
            if(current_participant->state == BattleParticipant::TURN) current_participant->OnTurn();
            if(current_participant->state == BattleParticipant::RETREAT) current_participant->OnRetreat();
        }
        
        
    }

    BattleParticipant* GetCurrentOpponent(){
        if(battle_participant_handles.size() > 0){
            return battle_participant_handles[turn%battle_participant_handles.size()];
        }
        return nullptr;
    }
};

#endif