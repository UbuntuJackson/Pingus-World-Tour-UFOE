#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include "../json/json_variant.h"

class Profile{
public:
    JsonDictionary save_file;
    std::string file_path_and_name = "";
    
    //The scene you left the game in. (unused)
    std::string level = "";

    std::string description = "";

    Profile(std::string _file_path_and_name);

    void Save();

    void Load();
};

#endif