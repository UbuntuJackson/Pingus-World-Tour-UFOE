#ifndef UFO_OBJECT_H
#define UFO_OBJECT_H
#include <functional>
class ufoObjectBase{
public:
    ufoObjectBase() = default;
    virtual void Update(){}
    virtual void OnUpdate(){}
};

template<typename tType>
class ufoObject : public ufoObjectBase{
public:
    tType* handle;
    ufoObject(tType* _handle, std::function<void(tType*)> _task):
    handle{_handle},
    task{_task}{}
    std::function<void(tType*)> task;
    void OnUpdate(){
        task(handle);
    }
};

#endif