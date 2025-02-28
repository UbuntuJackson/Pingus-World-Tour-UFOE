import os
import importlib
import json

var_exporter_module = importlib.import_module("UFO-Engine.tools.UFO_variable_exporter")

class Class:

    def __init__(self,_name, _header_file, _source_file = None, _base_class = None, *args):
        self.editor_attributes = []
        self.name = _name
        self.header_file = _header_file
        self.source_file = _source_file
        self.base_class = _base_class
        self.constructor_parameters = []

        for constructor_parameter in args:
            self.constructor_parameters.append(constructor_parameter)

    def export_variables_to_editor(self):
        exported_variables = var_exporter_module.export_variables(self.header_file)
        var_exporter_module.add_exported_variables_to_project(self,exported_variables)

    def add_attribute(self, _arg_type, _arg_name):
        self.editor_attributes.append((_arg_type,_arg_name))        
    
    def generate_class(self):
        pass

    def generate_json_to_cpp_bridge(self, _index):
        default_actor_attributes = ["width", "height"]
        attribute_map = {
            "width": "shape.size.x",
            "height": "shape.size.y"
        }
        #Get classes
        code : str = ""

        code += "    " + "case " + str(_index) + ":{\n"
        code += "    "*3 + "auto instance = _level->NewActor<"+self.name+">"+ "(" + "Vector2f(_actor_json.Get(\"x\").AsFloat(), _actor_json.Get(\"y\").AsFloat()));\n"
        for arg in self.editor_attributes:
            if arg[1] in default_actor_attributes: code += "    "*3 + "instance->"+ attribute_map[arg[1]]+ " = _actor_json.Get(\"" + arg[1] + "\").As" + arg[0]+'();\n'

        code += "            if(_actor_json.KeyExists(\"properties\")){\n"
        code += "                JsonArray& property_json = _actor_json.Get(\"properties\").AsArray();\n"
        code += "                for(auto&& _json : property_json.Iterable()){\n"

        code += "                   auto property_dict = _json->AsDictionary();\n"

        #For attributes that are in the properties object.
        for arg in self.editor_attributes:
            if not arg[1] in default_actor_attributes:
                code += '                    if(property_dict.Get(\"name\").AsString() == "' +arg[1] +'"){\n'
                code += '                        instance->'+ arg[1]+ ' = property_dict.Get(\"value\").As'+arg[0]+'();\n'
                code += '                    }\n'

                #code+= '                    if(_json.GetAsString("name") == "' +arg[1] +'"){\n'
                #code+= '                        instance->'+ arg[1]+ ' = _json.GetAs'+ arg[0] + '(\"'+ 'value' + '\");\n'
                #code+= '                    }\n'
        
        code+= '                }\n'
        code+= '            }\n'

        code+= "    "*2 + "}\n        break;\n"
        return code

class ProjectManager:

    def __init__(self):
        self.project_path = os.getcwd()
        self.classes = []
        self.images = []
        self.audio_tracks = []
        self.source_files = []

    def purge_attribute(_klass_name, _arg_name):

        level_dictionary = {}

        for layer in level_dictionary["layers"]:
            if layer["type"] == "ObjectLayer":
                for object in layer["objects"]:
                    for custom_property in object["properties"]:
                        if(custom_property["name"] == _arg_name):
                            del custom_property

    def generate_project(self):
        
        type_index : int = 0

        actor_json_bridge_dot_h = ""

        actor_json_bridge_dot_h += "#include <json_variant.h>\n"
        actor_json_bridge_dot_h += "#include <ufo_maths.h>\n"
        actor_json_bridge_dot_h += "#include <console.h>\n"

        for klass in self.classes:
            if klass == None: continue
            actor_json_bridge_dot_h += "#include " + '"../' + klass.header_file + '"\n'

        actor_json_bridge_dot_h += "void GeneratedActorJsonBridge(Level* _level, JsonDictionary& _actor_json, std::string _actor_sheet){\n"

        actor_json_bridge_dot_h += "    auto actors_tileset_data = _level->tilemap.GetTilesetData(_actor_sheet);\n"
        actor_json_bridge_dot_h += "    int type_id = _actor_json.Get(\"gid\").AsInt() - actors_tileset_data.tileset_start_id+1;\n"
        actor_json_bridge_dot_h += "    switch(type_id){\n"

        for klass in self.classes:
            type_index+=1
            if klass == None: continue
            actor_json_bridge_dot_h += "    " + klass.generate_json_to_cpp_bridge(type_index)
        
        actor_json_bridge_dot_h += "    }\n}\n"

        f = open(self.project_path+"/src/generated_actor_json_bridge.h","w+")
        f.write(actor_json_bridge_dot_h)
        f.close()

        cmake_lists_dot_txt = "cmake_minimum_required(VERSION 3.10)\nproject(OUT)\nadd_subdirectory(UFO-Engine) #CMakeLists.txt for engine is in folder called \"engine\"\nset(\n    SRC\n"
        
        for source_file in self.source_files:
            cmake_lists_dot_txt += "    " + source_file + "\n"
        
        for klass in self.classes:
            if klass == None: continue

            if klass.source_file != None: cmake_lists_dot_txt += "    " + klass.source_file + "\n"

        cmake_lists_dot_txt += ")\n"

        cmake_lists_dot_txt += "add_executable(${PROJECT_NAME} ${SRC})\ntarget_link_libraries(${PROJECT_NAME} UFO)"
        
        f = open(self.project_path+"/CMakeLists.txt","w+")
        f.write(cmake_lists_dot_txt)
        f.close()

        #Write a game_config file

        game_config_dict = {
            "resolution" : {
                "x" : 680,
                "y" : 480
            },
            "pixel-size" : {
                "x" : 2,
                "y" : 2    
            },
            "vsync" : True,
            "start-level" : "",
            "all-shapes-visible" : False,
            "edit-mode" : False,
            "modified-collision-lines-visible" : False
        }

        if not os.path.exists(self.project_path+"/"+"game_config.json"):
            game_config_file = open(self.project_path+"/"+"game_config.json", "x")
            game_config_file.write(json.dumps(game_config_dict, indent=4))
            game_config_file.close()

    #build takes arguments
    #_cmake_args - Arguments for cmake, such as debug flags like -ggdb to set optimisation level
    # for debugging with GDB
    #_generator_option - For example -G Ninja if Ninja should be used. Can be left empty if using make
    #_make_or_ninja - ninja if using ninja. make if using make.
    #_make_or_ninja_args - -j6 for using additional cores to compile. Works with both ninja and make.

    #Example
    #project.generate_project("-ggdb", "-G Ninja", "ninja" ,"-j6")
    def build(self,_cmake_args, _generator_option, _make_or_ninja , _make_or_ninja_args):
        ret = os.system("cd build && cmake .." + " " + _generator_option + " " + "-DCMAKE_CXX_FLAGS=" + '\"' + _cmake_args + '\"' + "&&" + _make_or_ninja + " " + _make_or_ninja_args)
        return (not ret)
        

    def add_source(self,_str):
        self.source_files.append(_str)
    
    def run(self):
        os.system("cd build && ./OUT")
    
    def debug(self):
        os.system("cd build && gdb OUT")