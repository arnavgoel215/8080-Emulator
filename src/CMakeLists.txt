#######################################################
# @file CMakeLists.txt
# @brief: Controller library project.
#
# @author: Jesse
# @author: Sergio Chavarria
# 
#######################################################

# Source directories.
set(CONTROLLER_PATH ${CMAKE_CURRENT_LIST_DIR}/controller)
set(MODEL_PATH ${CMAKE_CURRENT_LIST_DIR}/model)
set(VIEW_PATH ${CMAKE_CURRENT_LIST_DIR}/view)


# All subdirectories will have access to source tree.
include_directories(
    ${CONTROLLER_PATH}
    ${MODEL_PATH}
    ${VIEW_PATH}
)

# --- Qt ---
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Allow Qt metaobject compilation.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Source subdirectories.
add_subdirectory(controller)
add_subdirectory(model)
add_subdirectory(view)

# Main application executable
add_executable(space_invaders_emulator main.cpp)

# Link libraries
target_link_libraries(space_invaders_emulator
    PUBLIC
    controller
    emulator
    view
    Qt${QT_VERSION_MAJOR}::Widgets
)

# --- CLI Runner ---
# Executable for the command-line version of the emulator
add_executable(cli_emulator cli_runner.cpp)

# Link libraries for the CLI runner
target_link_libraries(cli_emulator
    PRIVATE
    controller
    emulator
    Qt${QT_VERSION_MAJOR}::Widgets
)
