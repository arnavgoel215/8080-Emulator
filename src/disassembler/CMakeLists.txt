#######################################################
# @file CMakeLists.txt
# @brief: Disassembler library project.
#
# @author: Sergio Chavarria - chavarse@oregonstate.edu
# @author: <TODO: ADD AUTHORS>
# 
#######################################################

# TODO: Add/Delete any needed .cpp files here.
add_library(disassembler STATIC)

# Set up source files.
target_sources(disassembler
    PUBLIC
    # <<<< ADD ANY required .cpp files in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}/disassembler.cpp
)

# Set up include directories.
# (Recommended to keep it self contained in this folder.)
target_include_directories(disassembler
    PUBLIC
    # <<<< ADD ANY required include directories in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}
)

# Test executable.
# NOTE I: Use this executable only for development, 
# since the project will use the static library instead.
add_executable(disassembler_main disassembler_main.cpp)
target_link_libraries(disassembler_main
    PUBLIC
    disassembler
)