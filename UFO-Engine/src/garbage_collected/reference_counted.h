#ifndef REF_COUNTED_H
#define REF_COUNTED_H

#include "../console/console.h"

template<typename tType>
class ReferenceCounted{
public:
    tType* object = nullptr;
    int id = 0;

    ReferenceCounted(){
        //object->reference_count++;
        Console::Out("default-constructed, create ID:", id);
    }


    ReferenceCounted(tType* _object){
        //if(_object != nullptr){
            Console::Out("create ID:", id);
            object = _object;
            _object->reference_count++;
            Console::Out(object->reference_count, "ref count up");
        //}
    }

    ReferenceCounted(const ReferenceCounted& _copy){
        object = _copy.object;
        object->reference_count++;
        Console::Out(object->reference_count, "User define copy constructor?");
    }

    ReferenceCounted& operator=(const ReferenceCounted& _copy){
        object = _copy.object;
        object->reference_count++;
        if(&_copy == this){
            Console::Out("copy is this");
            //return *this;
        }
        
        return *this;
    }

    ReferenceCounted Reference(){
        Console::Out("User define copy constructor?");
        return GarbageCollected(object);
    }

    ~ReferenceCounted(){
        Console::Out("destruct ID:", id);
        object->reference_count--;
        Console::Out(object->reference_count, "ref count down");
        if(object->reference_count == 0){
            Console::Out("[GarbageCollected<T>::~GarbageCollected()] An object was garbage collected.");
            delete object;
        }
    }
};

#endif