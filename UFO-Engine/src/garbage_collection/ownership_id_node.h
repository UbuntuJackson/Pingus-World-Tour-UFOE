#ifndef OWNERSHIP_ID_NODE_H
#define OWNERSHIP_ID_NODE_H
#include <vector>
#include <unordered_map>
#include <memory>
#include "../console/console.h"

struct OwnerShipID_Node{
    OwnerShipID_Node(){
        //Console::Out("Constructor ran", "OwnerShipID_Node");
    }
    std::unordered_map<int, std::unique_ptr<OwnerShipID_Node>> property;
};

#endif