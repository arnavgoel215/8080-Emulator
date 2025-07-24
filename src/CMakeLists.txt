add_subdirectory(controller)
add_subdirectory(disassembler)
add_subdirectory(model)
add_subdirectory(view)

# --- Qt ---
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Main application executable
add_executable(space_invaders_emulator main.cpp)

# Link libraries
target_link_libraries(space_invaders_emulator
    PRIVATE
    controller
    emulator
    view
    Qt${QT_VERSION_MAJOR}::Widgets
)
