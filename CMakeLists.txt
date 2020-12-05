cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

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

set(CMAKE_CXX_CLANG_TIDY "")

# Freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} Freetype::Freetype)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} GLEW::GLEW)

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

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
target_link_libraries("ImGui" glfw GLEW::GLEW)
target_link_libraries(${CMAKE_PROJECT_NAME} "ImGui")

# JSON
include_directories("lib/nlohmann")

# OpenGL
if (MSYS)
	find_library(OPENGL_gl_LIBRARY opengl32)
	target_link_libraries(${CMAKE_PROJECT_NAME} opengl32)
else()
	find_package(OpenGL REQUIRED)
	target_link_libraries(${CMAKE_PROJECT_NAME} OpenGL::GL)
endif()

# PortAudio
# find_package(portaudio REQUIRED)
#target_link_libraries(${CMAKE_PROJECT_NAME} portaudio)
add_subdirectory("lib/portaudio" EXCLUDE_FROM_ALL)
target_link_libraries(${CMAKE_PROJECT_NAME} portaudio)

include_directories("lib/SimpleINI")
add_library("SimpleINI" STATIC "lib/SimpleINI/ConvertUTF.c")
target_link_libraries(${CMAKE_PROJECT_NAME} "SimpleINI")

# stb_image
include_directories("lib/stb")

# Vorbis
find_library(VORBISFILE_LIBRARY vorbisfile)
if (VORBISFILE_LIBRARY)
	message(STATUS "Found VorbisFile: ${VORBISFILE_LIBRARY}")
else()
	message(FATAL_ERROR "Could NOT find VorbisFile library")
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} ${VORBISFILE_LIBRARY})

# std thread
target_link_libraries(${CMAKE_PROJECT_NAME} pthread)
