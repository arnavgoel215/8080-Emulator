#######################################################
# @file CMakeLists.txt
# @brief: Emulator (Model) library project.
#
# @author: Sergio Chavarria - chavarse@oregonstate.edu
# @author: <TODO: ADD AUTHORS>
# 
#######################################################

# TODO: Add/Delete any needed .cpp files here.
add_library(emulator STATIC)

# Set up source files.
target_sources(emulator
    PUBLIC
    # <<<< ADD ANY required .cpp files in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}/emulator.cpp
)

# Set up include directories.
# (Recommended to keep it self contained in this folder.)
target_include_directories(emulator
    PUBLIC
    # <<<< ADD ANY required include directories in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}
)

# Test executable.
# NOTE I: Use this executable only for development, 
# since the project will use the static library instead.
add_executable(emulator_main emulator_main.cpp)
target_link_libraries(emulator_main
    PUBLIC
    emulator
)