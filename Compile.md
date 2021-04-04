<!--
SPDX-FileCopyrightText: 2021 MatteoGodzilla

SPDX-License-Identifier: GPL-3.0-or-later
-->

﻿# How to setup the working environment

Before being able to compile (and work) on the project you need to setup a few things.

## Windows

### Install MSYS2

For windows we use MSYS2 as the basic shell environment, this tool will also allow you to easily install all the necessary libraries and tools.
Go to [the MSYS2 homepage](https://www.msys2.org/) and download the latest version for your system.
Follow the installation wizard and, after the installation is complete, open the MSYS2 shell (it should be in your start menu under the name of "MSYS2 MSYS").

Once in the terminal proceed to update the repositories by running:

``` bash
pacman -Syu
```

After that the program will ask to forcefully close the window, do so as it's required by MSYS2.
Reopen the terminal and continue updating with the following command:

``` bash
pacman -Su
```

### Install compilation tools and libraries for MSYS2

To install all the required tools and libraries you just have to run the following command in a MSYS2 shell.

``` bash
pacman -S mingw-w64-x86_64-gcc cmake make mingw-w64-x86_64-clang mingw-w64-x86_64-clang-tools-extra mingw-w64-x86_64-freetype mingw-w64-x86_64-glew mingw-w64-x86_64-glfw mingw-w64-x86_64-libvorbis mingw-w64-x86_64-mesa mingw-w64-x86_64-sfml
```

(MSYS2 has some dependency problems with clang-tidy and to fix it both gcc and clang need to be installed)

### Compile the project

Download or clone the project in a directory of your choice, for example I will refer to this directory as `C:\Dj-Engine`.
From the terminal, move to the project directory (path and directory name will most likely be different for you)

``` bash
cd "C:\Dj-Engine"
```

Now to compile the program just launch the build script:

``` bash
./build.sh
```

If everything goes well an executable `Dj-Engine.exe` will appear in the folder.
In case of problems some errors may show up on your shell window, if that is the case it most likely means that you forgot to install all the libraries and tools or you messed up some other step. Please refrain from contacting us as soon as the error shows up, try to redo all the steps (you will need to uninstall and do a fresh install of MSYS2).
If the error persists you can open an issue on the github page, explaining your situation, system details, exact steps you took and the error message you encounter.

### Execution

Before running the program for the first time you have to run the script

``` bash
./init.sh
```

this will put the necessary DLLs into the project directory. This step is only necessary the first time you run the program.
Now you can run the game with the command

``` bash
./Dj-Engine.exe
```

or by directly double clicking the executable.

## GNU/Linux

To install all the required tools and libraries you just have to run the following command, depending from the distro:

Arch-based:

``` bash
sudo pacman -S gcc cmake make clang-tools-extra freetype2 glew libvorbis sfml

# Install this if you're using x11 as window manager (most likely)
sudo pacman -S glfw-x11

# Install this if you're using wayland as window manager
sudo pacman -S glfw-wayland
```

Fedora:

``` bash
sudo dnf install gcc cmake make clang-tools-extra freetype-devel glew-devel glfw-devel libvorbis-devel SFML-devel
```

Ubuntu-based:

``` bash
sudo apt install gcc cmake make clang-tidy libfreetype6-dev libglew-dev libglfw3-dev libvorbis-dev libsfml-dev

```

Now to compile the program just launch the build script:

``` bash
./build.sh
```
