rm Dj-Engine
mkdir build-dir
cd build-dir
cmake ..
make
cp Dj-Engine ../Dj-Engine
cd ..
rm -r build-dir
DRI_PRIME=1 ./Dj-Engine
