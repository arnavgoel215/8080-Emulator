@REM Create build directory.
if not exist build (
    mkdir build
)

cd build
cmake ..
make
cd ..