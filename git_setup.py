import os

ret : int = os.system("git submodule update --init --recursive")

if ret:
    print("Git repository fully set up")