echo "- debug.sh -"

echo "Running dependencies.sh"
sh dependencies.sh

echo "Configuring build type flag..."
cd build
cmake -DCMAKE_BUILD_TYPE=Debug
echo "Generating cmake project..."
cmake -B . -S ../
cd ..

echo "Done!"