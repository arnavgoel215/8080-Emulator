#######################################################
# @file CMakeLists.txt
# @brief: Controller library project.
#
# @author: Sergio Chavarria - chavarse@oregonstate.edu
# @author: <TODO: ADD AUTHORS>
# 
#######################################################

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# TODO: Add/Delete any needed .cpp files here.
add_library(controller STATIC)

# Set up source files.
target_sources(controller
    PUBLIC
    # <<<< ADD ANY required .cpp files in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}/controller.cpp
)

# Set up include directories.
# (Recommended to keep it self contained in this folder.)
target_include_directories(controller
    PUBLIC
    # <<<< ADD ANY required include directories in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(controller PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Test executable.
# NOTE I: Use this executable only for development, 
# since the project will use the static library instead.
add_executable(controller_main controller_main.cpp)
target_link_libraries(controller_main
    PUBLIC
    controller
    emulator
    Qt${QT_VERSION_MAJOR}::Widgets
)