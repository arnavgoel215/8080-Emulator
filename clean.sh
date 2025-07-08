# Only clean if previous build is active.
if [ -d "build" ]; then
  cd build
  make clean
  cd ..
fi