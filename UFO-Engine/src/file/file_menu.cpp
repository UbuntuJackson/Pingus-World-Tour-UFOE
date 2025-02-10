#include <memory>
#include <filesystem>
#include <functional>
#include "../widgets/button.h"
#include "../widgets/wrap_menu.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_engine/ufo_engine.h"
#include "../level/level.h"
#include "../pingus/pingus_level.h"
#include "../widgets/label.h"
#include "../file/file.h"
#include "file_menu_button.h"
#include "../ufo_maths/ufo_maths.h"
#include "file_menu.h"

FileMenu::FileMenu(Vector2f _local_position, Vector2f _size, std::string _path) :
    WrapMenu(_local_position, _size),
    path{_path}
{
    
}

void FileMenu::SetDirectory(std::string _path){
    path = _path;
    selected_index = 0;

    for(auto&& child : children){
        child->QueueForPurge();
    }
    buttons.clear();

    for(const auto& directory_entry : std::filesystem::directory_iterator{_path}){
        Console::Out("Directory entry found");

        std::error_code ec;
        std::string s_path = std::string(directory_entry.path());
        if(std::filesystem::is_directory(directory_entry,ec)){

            std::string directory_name = s_path.substr(s_path.find_last_of("/")+1);

            auto b = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 150.0f), directory_name, directory_entry.path());
            b->background_colour = olc::Pixel(150,150,80);

            b->on_pressed = [&](Widget* _w, Button* _button){

                auto fm = dynamic_cast<FileMenu*>(_w);
                fm->path_stack.push_back(fm->path);

                fm->SetDirectory(dynamic_cast<FileMenuButton*>(_button)->path);
            };

            AddChild(std::move(b));
        }
        else{
            Console::Out("is file");

            std::string file_name = s_path.substr(s_path.find_last_of("/")+1);
            auto b = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 150.0f),file_name, directory_entry.path());
            b->background_colour = olc::Pixel(100,100,100);
            
            b->on_pressed = [&](Widget* _w, Button* _button){

                auto fm = dynamic_cast<FileMenu*>(_w);

                std::string file_path = dynamic_cast<FileMenuButton*>(_button)->path;
                fm->func_on_file_open(_w, dynamic_cast<FileMenuButton*>(_button), file_path.substr(file_path.find_last_of(".")+1));
                fm->OnFileOpen(dynamic_cast<FileMenuButton*>(_button), file_path.substr(file_path.find_last_of(".")+1));
            };
            AddChild(std::move(b));
        }

    }

    auto bback = std::make_unique<FileMenuButton>(Vector2f(0.0f, 0.0f),Vector2f(250.0f, 150.0f), "..", path);

    bback->background_colour = olc::Pixel(200,90,0);

    bback->on_pressed = [](Widget* _w, Button* _button){
        auto fm = dynamic_cast<FileMenu*>(_w);
        Console::Out(fm->path_stack.back());      
        fm->SetDirectory(fm->path_stack.back());
        fm->path_stack.pop_back();
    };
    AddChild(std::move(bback));

    func_on_file_menu_created(this);
    OnFileMenuCreated();

    for(const auto& child : new_children_queue){
        Button* b = dynamic_cast<Button*>(child.get());
        if(b != nullptr) buttons.push_back(b);
        Console::Out("Button Added");
    }

    float total_height = 0.0f;

    for(const auto& button : buttons){
        button->local_position.y = total_height;
        total_height+=button->rectangle.size.y;
        total_height+=spacing;
    }

    rectangle.size.y = total_height;
    rectangle.size.x = 0.0f;

    original_position = local_position;
}

void FileMenu::OnLevelEnter(Level* _level){
    
    level = _level;
    
    OnFileMenuCreated();

    SetDirectory(path);

    //WrapMenu::OnLevelEnter(_level);

}

void FileMenu::OnFileMenuCreated(){
    
}

void FileMenu::OnFileOpen(FileMenuButton* _button,std::string _file_extension){

}