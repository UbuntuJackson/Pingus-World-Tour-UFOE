import importlib

#Importing the tool

tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")

project = tool.ProjectManager()

goal = tool.Class("Goal", "src/goal.h", "src/goal.cpp")

#width and height are treated a bit weirdly for now, but it works.
goal.editor_attributes.append(("Int", "width"))
goal.editor_attributes.append(("Int", "height"))
#Enables the @export -keyword
goal.export_variables_to_editor()

main_menu = tool.Class("PingusMainMenu", "src/pingus_main_menu.h", "src/pingus_main_menu.cpp")

honey_coin = tool.Class("HoneyCoin", "src/honey_coin.h")

static_quadtree_actor = tool.Class("StaticQuadTreeActor","src/test/static_quadtree_actor.h")
static_quadtree_actor.export_variables_to_editor()

dynamic_quadtree_actor = tool.Class("DynamicQuadTreeActor","src/test/dynamic_quadtree_actor.h")
dynamic_quadtree_actor.export_variables_to_editor()

#Append all Actors that we want to implement
project.classes.append(honey_coin)
project.import_actor("src/pingu.h")
project.classes.append(None)
project.import_actor("src/spawner.h")
project.classes.append(goal)
project.classes.append(main_menu)
project.import_actor("src/item_select_menu.h", "src/item_select_menu.cpp")
project.classes.append(None)
project.classes.append(static_quadtree_actor)
project.classes.append(dynamic_quadtree_actor)
project.import_actor("src/world_map_location.h","src/world_map_location.cpp")
project.import_actor("src/test/movable_camera.h", "src/test/movable_camera.cpp")
project.import_actor("src/world_map_cursor.h","src/world_map_cursor.cpp")
project.import_actor("src/world_map.h","src/world_map.cpp")
project.import_actor("src/pingus_splash_screen.h")
project.import_actor("src/story_mode_intro.h")
project.import_actor("src/pingus_world_tour_editor.h","src/pingus_world_tour_editor.cpp")

#Source-files that aren't Actors.
project.source_files.append("game.cpp")
project.source_files.append("src/level_select_menu.cpp")
project.source_files.append("src/pingus_level.cpp")
project.source_files.append("src/new_game_menu.cpp")
project.source_files.append("src/load_game_menu.cpp")
project.source_files.append("src/pwt_editor_layer_button.cpp")
project.source_files.append("external/stb_image_write_implementation.cpp")

#Generate generated_actor_json_bridge.h and CMakeLists.txt.
project.generate_project()