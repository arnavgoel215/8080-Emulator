# Only clean if previous build is active.
if [ -d "build" ]; then
  cd build
  cmake --build . -- clean
  cd ..
fi