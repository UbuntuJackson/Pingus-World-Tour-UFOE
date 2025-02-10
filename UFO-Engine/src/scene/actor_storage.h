//UNUSED

#ifndef ACTOR_STORAGE_H
#define ACTOR_STORAGE_H

class ActorStorage{
public:

    ActorStorage() = default;
    std::unordered_map<int, std::unique_ptr<Camera>> cameras; //owner, camera
    std::vector<int> camera_hierarchy;
    std::unordered_map<int, std::pair<std::unique_ptr<ufoSprite>, bool>> ufo_sprites;
    std::unordered_map<int, std::unique_ptr<Circle>> circles;
    std::unordered_map<int, std::pair<std::unique_ptr<Rectangle>, bool>> rectangles;
    std::unordered_map<int, std::unique_ptr<Coin>> coins;
    std::unordered_map<int, std::unique_ptr<PlayerBody>> player_bodies;
    std::unordered_map<int, std::unique_ptr<TopDownBody>> top_down_bodies;

    std::unordered_map<int, std::unique_ptr<ufoRay<Ray2>>> line_segments;
    std::unordered_map<int, std::unique_ptr<ufoRay<Ray2>>> user_rays;
    std::unordered_map<int, std::unique_ptr<ufoObjectBase>> ufo_objects; //rename this to something
    std::unordered_map<int, std::unique_ptr<Button>> buttons;
    std::unordered_map<int, std::unique_ptr<AnimationBundleReference>> animation_bundle_references;
    std::unordered_map<int, std::unique_ptr<DynamicShape<Rectangle>>> dynamic_rectangles;
    std::unordered_map<int, std::unique_ptr<ColourShapeBase>> colour_shapes;

};

#endif