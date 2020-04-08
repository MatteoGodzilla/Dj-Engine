if [[ -d /mingw64/bin ]]
then
	export PATH=/mingw64/bin:$PATH
fi
if [ "$1" == "-f" ]
then
	rm -r build-dir
fi
mkdir build-dir
cd build-dir
cmake .. && make
cp Dj-Engine ../
