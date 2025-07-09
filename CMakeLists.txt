#######################################################
# @file CMakeLists.txt
# @brief: Root configuration for 8080 Emulator project.
# 
# This handles all the subprojects including:
# - Model: 8080 Emulator code.
# - View: GUI Qt interface.
# - Controller: Emulator interface definitions.
# - Disassembler: Utility to disassemble 8080 ROM files.
#
# @author: Sergio Chavarria - chavarse@oregonstate.edu
# @author: <TODO: ADD AUTHORS>
# 
#######################################################

# Baseline project configuration.
cmake_minimum_required(VERSION 3.16) # Minimum version required by Qt.
project(space_invaders_emulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Copy all executables and libraries to ./out.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out)

# Set Debug flags by default.
set(CMAKE_BUILD_TYPE DEBUG)

# Subprojects.
add_subdirectory(src)

# TODO: Link all project libraries into a single executable.