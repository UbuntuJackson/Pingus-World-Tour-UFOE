import importlib
import platform

print(platform.system())

tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")

project = tool.ProjectManager()

pingu = tool.Class("Pingu", "src/pingu.h")

goal = tool.Class("Goal", "src/goal.h", "src/goal.cpp")
goal.editor_attributes.append(("Int", "number_of_pingus"))
goal.editor_attributes.append(("Int", "width"))
goal.editor_attributes.append(("Int", "height"))

main_menu = tool.Class("PingusMainMenu", "src/pingus_main_menu.h", "src/pingus_main_menu.cpp")

project.classes.append(None)
project.classes.append(pingu)
project.classes.append(None)
project.classes.append(None)
project.classes.append(goal)
project.classes.append(main_menu)

project.source_files.append("game.cpp")
project.source_files.append("src/level_select_menu.cpp")
project.source_files.append("src/pingus_level.cpp")

project.generate_project("-ggdb", "-j6")

project.run()