import importlib

#Importing the tool

tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")

project = tool.ProjectManager()

#Creating variables for data about the Actor-classes
pingu = tool.Class("Pingu", "src/pingu.h")

goal = tool.Class("Goal", "src/goal.h", "src/goal.cpp")

#width and height are treated a bit weirdly for now, but it works.
goal.editor_attributes.append(("Int", "width"))
goal.editor_attributes.append(("Int", "height"))
#Enables the @export -keyword
goal.export_variables_to_editor()

main_menu = tool.Class("PingusMainMenu", "src/pingus_main_menu.h", "src/pingus_main_menu.cpp")

spawner = tool.Class("Spawner", "src/spawner.h")
spawner.export_variables_to_editor()

item_select_menu = tool.Class("ItemSelectMenu", "src/item_select_menu.h", "src/item_select_menu.cpp")
item_select_menu.export_variables_to_editor()

honey_coin = tool.Class("HoneyCoin", "src/honey_coin.h")

#Append all Actors that we want to implement
project.classes.append(honey_coin)
project.classes.append(pingu)
project.classes.append(None)
project.classes.append(spawner)
project.classes.append(goal)
project.classes.append(main_menu)
project.classes.append(item_select_menu)

#Source-files that aren't Actors.
project.source_files.append("game.cpp")
project.source_files.append("src/level_select_menu.cpp")
project.source_files.append("src/pingus_level.cpp")

#Generate generated_actor_json_bridge.h and CMakeLists.txt.
project.generate_project()
build_succeed = project.build("-ggdb", "", "make" ,"-j6")

#This should only run if build succeeds though?
if build_succeed: project.run()
else: print("Build failed.")