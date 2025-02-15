import importlib

tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")

project = tool.ProjectManager()

pingu = tool.Class("Pingu", "src/pingu.h")

goal = tool.Class("Goal", "src/goal.h", "src/goal.cpp")
goal.editor_attributes.append(("Int", "number_of_pingus"))
goal.editor_attributes.append(("Int", "width"))
goal.editor_attributes.append(("Int", "height"))
goal.editor_attributes.append(("Int", "maximum_rescuable_pingus"))

main_menu = tool.Class("PingusMainMenu", "src/pingus_main_menu.h", "src/pingus_main_menu.cpp")

spawner = tool.Class("Spawner", "src/spawner.h")
spawner.editor_attributes.append(("Int","number_of_pingus"))

item_select_menu = tool.Class("ItemSelectMenu", "src/item_select_menu.h", "src/item_select_menu.cpp")
item_select_menu.editor_attributes.append(("Int", "number_of_builders"))
item_select_menu.editor_attributes.append(("Int", "number_of_bombers"))
item_select_menu.editor_attributes.append(("Int", "number_of_parachutes"))

honey_coin = tool.Class("HoneyCoin", "src/honey_coin.h")

project.classes.append(honey_coin)
project.classes.append(pingu)
project.classes.append(None)
project.classes.append(spawner)
project.classes.append(goal)
project.classes.append(main_menu)
project.classes.append(item_select_menu)

project.source_files.append("game.cpp")
project.source_files.append("src/level_select_menu.cpp")
project.source_files.append("src/pingus_level.cpp")

project.generate_project("-ggdb", "-j6")

project.run()