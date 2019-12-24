# Install script for directory: C:/Users/caten/Dj-Engine/lib/include/ogg

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/caten/Dj-engine/out/install/x86-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/ogg.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ogg" TYPE FILE FILES
    "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/include/ogg/config_types.h"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/include/ogg/ogg.h"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/include/ogg/os_types.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets.cmake"
         "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/CMakeFiles/Export/lib/cmake/Ogg/OggTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg/OggTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg" TYPE FILE FILES "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/CMakeFiles/Export/lib/cmake/Ogg/OggTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg" TYPE FILE FILES "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/CMakeFiles/Export/lib/cmake/Ogg/OggTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Ogg" TYPE FILE FILES
    "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/OggConfig.cmake"
    "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/OggConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/caten/Dj-Engine/out/build/x86-Debug/lib/include/ogg/ogg.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogg/html" TYPE FILE FILES
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/framing.html"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/index.html"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/oggstream.html"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/ogg-multiplex.html"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/fish_xiph_org.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/multiplex1.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/packets.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/pages.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/stream.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/vorbisword2.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/white-ogg.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/white-xifish.png"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/rfc3533.txt"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/rfc5334.txt"
    "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/skeleton.html"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogg/html" TYPE DIRECTORY FILES "C:/Users/caten/Dj-Engine/lib/include/ogg/doc/libogg")
endif()

