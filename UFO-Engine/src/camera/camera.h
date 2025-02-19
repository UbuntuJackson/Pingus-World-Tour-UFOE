#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/circle.h"
#include "../shapes/rectangle.h"
#include "../shapes/ray2.h"
#include "../sprite_reference/sprite_reference.h"
#include "../actor/actor.h"
#include "../shapes/pill.h"

struct ufoSprite;

enum CameraStates{
    BASIC_FOLLOW,
    RESTRICTED_FOLLOW,
    CLAMPED_FOLLOW,
    FOLLOW_PLATFORMER,
    FOLLOW, //Camera will be set to seem like it focuses on the center point of a sprite.
    SWITCH, //Transitions from one target to another
    MOVE, //Applies a movement vector to the camera position.
	    //This state would require some parameters I guess
    ZOOM,  //Transitions into a different scaling factor. Need to give the program a scaling factor
	    // Transition into
    MOUSE,
    MULTIPLAYER,
    ALL_CONTAINED,
    STATIC,
    PLATFORMER
};

struct Bounds{
    float x0;
    float x1;
    float y0;
    float y1;
    float GetWidthHalf(){return (x1-x0)/2.0f;} //something is wrong if this returns negative
    float GetHeightHalf(){return (y1-y0)/2.0f;} //something is wrong if this returns negative
};

class Camera : public Actor{
public:
    bool clamp = false;
    int target_id = -1; //target looks through cameras and takes the id matching with it's own
    Bounds view;
    Bounds world;
    float rotation;
    float scale;
    olc::vf2d original_local_position;
    olc::vf2d offset_from_local_position;
    bool local_position_set_elsewhere = true;

    Camera(olc::vf2d _position);
    void OnStart(Level* _level);
    void EarlyUpdate();
    void ClampLocalPosition();
    void HandleUpdate();
    olc::vf2d Transform(const olc::vf2d& _position);
    olc::vf2d TransformScreenToWorld(const olc::vf2d& _screen_position);
    Rectangle Transform(const Rectangle& _rectangle);
    Circle Transform(const Circle& _circle);
    Ray2 Transform(const Ray2& _ray);
    Pill Transform(const Pill& _pill);
    SpriteReference Transform(const SpriteReference& _sprite);
    bool IsOnScreen(olc::vf2d _position, olc::vf2d _offset = {0.0f, 0.0f});
    Rectangle GetOnScreenRectangleInWorld(olc::vf2d _offset = {0.0f, 0.0f});
};
#endif