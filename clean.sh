if [ -d "./build" ]; then
    cd build
    cmake --build . --target clean
    cd ..
fi

rm -rf raytracer
rm -rf build
rm -rf CMakeFiles
rm -rf makefile
rm -rf cmake_install.cmake
rm -rf compile_commands.json
rm -rf CMakeCache.txt
rm -rf bindings