echo "- release.sh -"

echo "Running dependencies.sh"
sh dependencies.sh

echo "Configuring build type flag..."
cd build
cmake -DCMAKE_BUILD_TYPE=Release
echo "Generating cmake project..."
cmake -B . -S ../
cd ..

echo "Running build.sh"
sh build.sh

echo "Done!"