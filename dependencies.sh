echo "- dependencies.sh -"

mkdir build
cd build
echo "Running conan install..."
conan install .. --build missing --profile default
cd ..
echo "Done!"