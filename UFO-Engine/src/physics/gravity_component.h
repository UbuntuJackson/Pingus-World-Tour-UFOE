class GravityComponent{
public:
    olc::vf2d &parent_velocity;
    olc::vf2d &parent_position;
    float magnitude;
    void Update(olc::vf2d _gravitational_direction){
        *parent_velocity += magnitude*_gravitational_direction;
    }
};