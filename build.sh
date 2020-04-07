if [[ -d /mingw64/bin ]]
then
	export PATH=/mingw64/bin:$PATH
fi
rm Dj-Engine
mkdir build-dir
cd build-dir
cmake ..
make
cp Dj-Engine ../Dj-Engine
cd ..
if [ "$1" == "-f" ]
then
	rm -r build-dir
fi
