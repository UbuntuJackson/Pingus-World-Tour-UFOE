import os
import subprocess

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
        code += "    "*3 + "auto instance = _level->NewActor<"+self.name+">"+ "(" + "Vector2f((float)(_actor_json.GetAsInt(\"x\")), (float)(_actor_json.GetAsInt(\"y\")))"
        code += ");\n"
        for arg in self.editor_attributes:
            if arg[1] in default_actor_attributes: code += "    "*3 + "instance->"+ attribute_map[arg[1]]+ " = _actor_json.GetAs" + arg[0] + "(\"" + arg[1] + "\");\n"

        code+= "            Console::Out("+ '"'+ self.name +'"' +");\n"

        #For attributes that are in the properties object.
        """for arg in self.editor_attributes:
            if not arg[1] in default_actor_attributes:

                code+= 'Json property_json = layer.GetObject("properties");\n'
                code+= 'Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){'
                code+= '    if(property_json.member){'
                code+= '        '
                code+= '        if(_json.GetAsString("name") == "parallax_x"){'
                code+= '            parallax_x = _json.GetAsDouble("value");'
                code+= '        }'
                code+= '        if(_json.GetAsString("name") == "parallax_y"){'
                code+= '            parallax_y = _json.GetAsDouble("value");'
                code+= '        }'
                code+= '    }'
                code+= '}'
                code+= ');'

                code += "    "*3 + "instance->"+ attribute_map[arg[1]]+ " = _actor_json.GetAs" + arg[0] + "(\"" + arg[1] + "\");\n"""

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

    def generate_project(self, _cmake_args = "", _make_args = ""):
        
        type_index : int = 0

        actor_json_bridge_dot_h = ""

        actor_json_bridge_dot_h += "#include <json.h>\n"
        actor_json_bridge_dot_h += "#include <ufo_maths.h>\n"
        actor_json_bridge_dot_h += "#include <console.h>\n"

        for klass in self.classes:
            if klass == None: continue
            actor_json_bridge_dot_h += "#include " + '"../' + klass.header_file + '"\n'

        actor_json_bridge_dot_h += "void GeneratedActorJsonBridge(Level* _level, Json& _actor_json, std::string _actor_sheet){\n"

        actor_json_bridge_dot_h += "    auto actors_tileset_data = _level->tilemap.GetTilesetData(_actor_sheet);\n"
        actor_json_bridge_dot_h += "    int type_id = _actor_json.GetAsInt(\"gid\") - actors_tileset_data.tileset_start_id+1;\n"
        actor_json_bridge_dot_h += "    Console::Out(type_id);"
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

        os.system("cd build && cmake .." + " " + "-DCMAKE_CXX_FLAGS=" + '\"' + _cmake_args + '\"' + "&& make " + _make_args)

    def add_source(self,_str):
        self.source_files.append(_str)
    
    def run(self):
        os.system("cd build && ./OUT")
    
    def debug(self):
        os.system("cd build && gdb OUT")