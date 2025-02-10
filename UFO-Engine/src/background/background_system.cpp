#include "background_system.h"
#include "../shapes/rectangle.h"
#include "../ufo_maths/ufo_maths.h"
#include "../camera/camera.h"
#include "../external/olcPixelGameEngine.h"
#include "../graphics_provider/graphics_provider.h"
#include "../asset_manager/asset_manager.h"

void BackgroundSystem::HandleUpdate(Camera* _camera){
    for(auto&& background_data : backgrounds){
        background_data.rectangle.position -= _camera->position*background_data.parallax;
        for(int i = _camera->view.x0-(_camera->view.x1-_camera->view.x0); i < _camera->view.x1+(_camera->view.x1-_camera->view.x0); i+=((int)background_data.rectangle.size.x)){
            GraphicsProvider::Get().DrawDecal(olc::vf2d(_camera->Transform({background_data.rectangle.position.x, 0.0f})),AssetManager::Get().GetDecal(background_data.key));
        }
    }
}