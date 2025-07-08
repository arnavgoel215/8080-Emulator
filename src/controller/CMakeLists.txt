#######################################################
# @file CMakeLists.txt
# @brief: Controller library project.
#
# @author: Sergio Chavarria - chavarse@oregonstate.edu
# @author: <TODO: ADD AUTHORS>
# 
#######################################################

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
)

# Test executable.
# NOTE I: Use this executable only for development, 
# since the project will use the static library instead.
# NOTE II: Using .exe extension even if we're on linux,
# Just to keep consistency with debug configurations.
add_executable(controller_main.exe controller_main.cpp)
target_link_libraries(controller_main.exe
    PUBLIC
    controller
)