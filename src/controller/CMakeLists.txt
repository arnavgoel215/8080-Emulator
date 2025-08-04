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
qt_add_library(controller
    STATIC
    ${CMAKE_CURRENT_LIST_DIR}/controller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/controller.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SoundManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SoundManager.hpp
)

# Set up include directories.
# (Recommended to keep it self contained in this folder.)
target_include_directories(controller
    PUBLIC
    # <<<< ADD ANY required include directories in here. >>>>
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(controller PUBLIC Qt${QT_VERSION_MAJOR}::Widgets emulator view)