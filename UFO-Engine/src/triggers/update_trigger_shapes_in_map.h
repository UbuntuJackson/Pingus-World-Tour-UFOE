template<typename tShape, typename tOtherShape>
void UpdateTriggerShapesInMap(Scene* _scene, std::unordered_map<int, std::unique_ptr<Trigger<tShape>>>* _triggers, std::unique_ptr<tTriggerChaser<tOtherShape>>>* _trigger_chasers){

    for(const auto& trigger : _triggers){
        for(const auto& trigger_chaser : _trigger_chasers){
            if(ufoPolymorphism::Overlaps(trigger.shape, trigger_chaser.shape)){
                ufoLang::Execute(_trigger->code);
                trigger_chaser->ufoLangBind(_scene, _trigger->code);
            }
        }
    }

}