cmake_minimum_required(VERSION 3.8)

project("Dj-Engine")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
	"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(warnings "-Wall -Werror -Wextra")
endif()

include_directories("header")

AUX_SOURCE_DIRECTORY(source SRCFOLDER)
add_executable(${CMAKE_PROJECT_NAME}
	"Dj-Engine.cpp"
	${SRCFOLDER}
)

# Freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} "freetype")

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLEW_LIBRARIES})

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} "glfw")

# GLM
set(glm_DIR ${CMAKE_SOURCE_DIR}/lib/glm/cmake/glm/)
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# IMGUI
include_directories("lib/IMGUI")
add_library("ImGui" STATIC
	"lib/IMGUI/imgui.cpp"
	"lib/IMGUI/imgui_draw.cpp"
	"lib/IMGUI/imgui_impl_glfw.cpp"
	"lib/IMGUI/imgui_impl_opengl3.cpp"
	"lib/IMGUI/imgui_widgets.cpp"
)
target_link_libraries(${CMAKE_PROJECT_NAME} "ImGui")

# JSON
include_directories("lib/nlohmann")

# OpenAL
find_package(OpenAL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} "openal")

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} "OpenGL::GL")

# SimpleINI
include_directories("lib/SimpleINI")
add_library("SimpleINI" STATIC "lib/SimpleINI/ConvertUTF.c")
target_link_libraries(${CMAKE_PROJECT_NAME} "SimpleINI")

# stb_image
include_directories("lib/stb")

# Vorbis
find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
target_link_libraries(${CMAKE_PROJECT_NAME} ${VORBISFILE_LIBRARY})

target_link_libraries(${CMAKE_PROJECT_NAME} "stdc++fs")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
