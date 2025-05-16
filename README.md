# Pingus-World-Tour-UFOE

Building instructions (Linux):
    
    Dependencies to install:
    libopengl-dev
    libglx-dev
    libpng-dev
    libx11-dev
    libthread-pool-dev (Uncertain if necessary)
    libopenal-dev (Uncertain if necessary)

    -python 3.10+

Build instructions (Windows):
    
    Dependencies to install:

    -python (version 3.10 or more)
    -Ninja Build
    -CMake
    -MingW (Just get the first one here: https://github.com/niXman/mingw-builds-binaries/releases)

    Make sure these three are added to PATH, in other words added as globally available commands.
    You can test them by going to your desktop, open the commandline and type
    ```
    python --version
    ninja --version
    cmake --version
    gcc --version
    ```

    Setting up the repository:

    Assuming you have git run the following commands in the commandline
    ```
    git clone https://github.com/UbuntuJackson/Pingus-World-Tour-UFOE.git
    ```
    Navigate into the repository, navigate to the unstable branch
    ```
    git checkout RectangleNameSpaced
    ```
    Run
    ```
    git submodule update --init --recursive
    ```

    Run start.py, which should set everything up and run the game. There is a chance the UFO-Project-Manager, compiler or linker complains.

