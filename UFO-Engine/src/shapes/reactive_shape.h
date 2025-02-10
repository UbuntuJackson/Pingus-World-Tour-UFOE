#ifndef REACTIVE_SHAPE_H
#define REACTIVE_SHAPE_H
#include <unordererd_map>

class ReactiveShapeBase{
public:
    ReactiveShapeBase(std::unordered_map<int, ReactiveShapeBase*> _group){}
    std::unordered_map<int, ReactiveShapeBase*> group;
    bool is_hit_this_frame = false;

    virtual void Update() = 0;
    void Reset(){
        is_hit_this_frame = false;
    }
    virtual bool IsOverlapping(Rectangle* _other_shape) = 0;
    virtual bool IsOverlapping(Circle* _other_shape) = 0;
    virtual bool IsOverlapping(Ray2* _other_shape) = 0;
};

template <typename tShape>
class ReactiveShape : public ReactiveShapeBase{
public:
    tShape* shape;

    ReactiveShape(tShape* _shape, std::unordered_map<int, ReactiveShapeBase*> _group) :
        shape{_shape},
        ReactiveShapeBase(_group)
        {}

    void AddGroupToCheck(std::unordered_map<int, Shape> *_map){
        groups.push_back(_map);
    }
    
    void Update(){
        
        for(const auto& [item : group]){
            if(item->IsOverlapping(shape)){ //this would not work because I'm not sure how to do the rest, polymorphism-wise.
                is_hit_this_frame = true;
            }
        }
        
    }

    bool IsOverlapping(Rectangle* _other_shape){
        return ufoMaths::IsOverlapping(*_other_shape, *shape);
    }
    bool IsOverlapping(Circle* _other_shape){
        return ufoMaths::IsOverlapping(*_other_shape, *shape);
    }
    bool IsOverlapping(Ray2* _other_shape){
        return ufoMaths::IsOverlapping(*_other_shape, *shape);
    }

};

#endif