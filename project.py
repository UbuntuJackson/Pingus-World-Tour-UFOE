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

#Append all Actors that we want to implement
honey_coin = project.import_actor("src/honey_coin.h")
pingu = project.import_actor("src/pingu.h")
spawner = project.import_actor("src/spawner.h")
project.classes.append(goal)
pingus_main_menu = project.import_actor("src/pingus_main_menu.h", "src/pingus_main_menu.cpp")
item_Select_menu = project.import_actor("src/item_select_menu.h", "src/item_select_menu.cpp")
static_quad_tree_actor = project.import_actor("src/test/static_quadtree_actor.h")
dynamic_quad_tree_actor = project.import_actor("src/test/dynamic_quadtree_actor.h")
world_map_location = project.import_actor("src/world_map_location.h","src/world_map_location.cpp")
movable_camera = project.import_actor("src/test/movable_camera.h", "src/test/movable_camera.cpp")
world_map_cursor = project.import_actor("src/world_map_cursor.h","src/world_map_cursor.cpp")
world_map = project.import_actor("src/world_map.h","src/world_map.cpp")
splash_screen = project.import_actor("src/pingus_splash_screen.h")
story_mode_intro = project.import_actor("src/story_mode_intro.h")
pwt_editor = project.import_actor("src/pingus_world_tour_editor.h","src/pingus_world_tour_editor.cpp")
moving_solid = project.import_actor("src/moving_solid.h","src/moving_solid.cpp")

project.categories["pingus_actor_sheet"] = [
    honey_coin,
    pingu,
    None,
    spawner,
    goal,
    pingus_main_menu,
    item_Select_menu,
    None,
    static_quad_tree_actor,
    dynamic_quad_tree_actor,
    world_map_location,
    movable_camera,
    world_map_cursor,
    world_map,
    splash_screen,
    story_mode_intro,
    pwt_editor,
    
]

project.categories["moving_solids"] = [
    moving_solid,
    moving_solid,
    moving_solid,
    moving_solid,
    moving_solid,
    moving_solid
]


#Source-files that aren't Actors.
project.source_files.append("game.cpp")
project.source_files.append("src/level_select_menu.cpp")
project.source_files.append("src/test/quadtree_rectangle.cpp")
project.source_files.append("src/pingus_level.cpp")
project.source_files.append("src/new_game_menu.cpp")
project.source_files.append("src/load_game_menu.cpp")
project.source_files.append("src/pwt_editor_layer_button.cpp")
project.source_files.append("external/stb_image_write_implementation.cpp")

#Generate generated_actor_json_bridge.h and CMakeLists.txt.
project.generate_project()