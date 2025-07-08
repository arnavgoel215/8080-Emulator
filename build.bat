@REM Create build directory.
if not exist build (
    mkdir build
)

cd build
@REM Use MinGW Generator, otherwise the default one is Visual Studio.
cmake .. -G "MinGW Makefiles"
cmake --build . --parallel
cd ..