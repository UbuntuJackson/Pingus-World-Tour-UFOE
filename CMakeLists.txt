cmake_minimum_required(VERSION 3.10)
project(OUT)
add_subdirectory(UFO-Engine) #CMakeLists.txt for engine is in folder called "engine"
set(
    SRC
    game.cpp
    src/level_select_menu.cpp
    src/pingus_level.cpp
    src/goal.cpp
    src/pingus_main_menu.cpp
    src/item_select_menu.cpp
)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} UFO)