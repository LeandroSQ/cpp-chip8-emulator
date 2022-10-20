echo "- build.sh -"

mkdir build
cd build
echo "Running cmake --build . -j 10"
cmake --build . -j 10 || {
    cd ..;
    exit 1;
}
cd ..