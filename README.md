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

    Build instructions for linux are currently incomplete, coming soon.

Build instructions (Windows):
    
    Step 1. Dependencies to install:

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

    Step 2. Setting up the repository:

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

    Step 3. Create a start.py file, here's a template:
    ```py
    from project import project

    build_succeed = project.build("-ggdb", "", "make" ,"-j6")

    #build takes arguments
    #_cmake_args - Arguments for cmake, such as debug flags like -ggdb to set optimisation level
    # for debugging with GDB
    #_generator_option - For example -G Ninja if Ninja should be used. Can be left empty if using make
    #_make_or_ninja - ninja if using ninja. make if using make.
    #_make_or_ninja_args - -j6 for using additional cores to compile. Works with both ninja and make.
 
    if build_succeed: project.run()
    else: print("Build failed.")
    ```

    Run start.py, which should set everything up and run the game. There is a chance the UFO-Project-Manager, compiler or linker complains.

    If you're on MSVC
    (For the MSVC chain)

    The UFO-Engine uses a bit of an unconventional pipeline to fetch some of it's metadata. The project.py file runs a function called
    'generate_project', which generates a file called generated.h and a CMakeLists.txt which accounts for MSVC. Therefore, start from the
    step where you update the git submodules (The end of step 2.), in the windows build instructions. From there, perform the following instructions:

    Run the project.py file,
    Inside the build folder there's a folder called UFO-Engine, in which there's a Visual Studio solution. All that has to be done is the solution file has to be opened and ran.
    Now you can do the same for the OUT.sln and it's done.