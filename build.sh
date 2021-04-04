#!/usr/bin/env bash

# SPDX-FileCopyrightText: 2021 MatteoGodzilla
#
# SPDX-License-Identifier: GPL-3.0-or-later

if [[ -d /mingw64/bin ]]
then
	export PATH=/mingw64/bin:$PATH
fi

FORCE=false
DEBUG=false
RELEASE=false

while getopts "fdrh" opt; do
  case ${opt} in
    f ) echo "FORCE BUILD"
		FORCE=true
      ;;
    d ) echo "DEBUG BUILD"
		DEBUG=true
      ;;
    r ) echo "RELEASE BUILD"
		RELEASE=true
      ;;
    h ) echo "Usage: build.sh [-f] [-d] [-r]"
        echo "-f : Remove cache files and force a clean build"
        echo "-d : Change build type to debug"
        echo "-r : Change build type to release"
        exit 0
      ;;
    \? ) echo "Usage: build.sh [-f] [-d] [-r]"
        echo "-f : Remove cache files and force a clean build"
        echo "-d : Change build type to debug"
        echo "-r : Change build type to release"
        exit 0
      ;;
  esac
done

if [ $FORCE = true ]
then
	rm -r build
fi
mkdir build
cd build
if [ $DEBUG = true ]
then
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make
elif [ $RELEASE = true ]
then
	cmake -DCMAKE_BUILD_TYPE=Release .. && make
else
	cmake .. && make
fi
cp Dj-Engine ..
