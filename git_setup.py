import importlib

tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")

project = tool.ProjectManager()

if project.git_setup():
    print("Git repository fully set up")