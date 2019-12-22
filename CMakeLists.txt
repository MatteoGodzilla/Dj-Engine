cmake_minimum_required(VERSION 3.8)

project("Dj-Engine")

include_directories("header")

include_directories("lib")

include_directories("lib/IMGUI")

include_directories("lib/GLEW/include")

include_directories("lib/ogg/include")

include_directories("lib/freetype/include/")

include_directories("lib/glm")

add_library("ImGui" STATIC
	"lib/IMGUI/imgui.cpp"
	"lib/IMGUI/imgui_demo.cpp"
	"lib/IMGUI/imgui_draw.cpp"
	"lib/IMGUI/imgui_impl_glfw.cpp"
	"lib/IMGUI/imgui_impl_opengl3.cpp"
	"lib/IMGUI/imgui_widgets.cpp"
)

set(OGG_INCLUDE_DIRS "lib/ogg/include")

add_subdirectory("lib/vorbis")

add_subdirectory("lib/openal-soft")

add_subdirectory("lib/freetype")

add_subdirectory("lib/GLFW")

add_subdirectory("lib/GLEW/build/cmake")

add_executable(${CMAKE_PROJECT_NAME} 
	"Dj-Engine.cpp" 
	"source/Audio.cpp"
	"source/Game.cpp"
	"source/GameRender.cpp"
	"source/Generator.cpp"
	"source/Graphics.cpp"
	"source/MenuNavigator.cpp"
	"source/MenuNode.cpp"
	"source/MenuRender.cpp"
	"source/Note.cpp"
	"source/Player.cpp"
	"source/Rendr.cpp"
	"source/SongScanner.cpp"

	"lib/vorbis/lib/vorbisfile.c"
)

target_link_libraries(${CMAKE_PROJECT_NAME} "glfw")

target_link_libraries(${CMAKE_PROJECT_NAME} "freetype")

target_link_libraries(${CMAKE_PROJECT_NAME} "ImGui")

target_link_libraries(${CMAKE_PROJECT_NAME} "glew")

target_link_libraries(${CMAKE_PROJECT_NAME} "OpenAL")

target_link_libraries(${CMAKE_PROJECT_NAME} "vorbis")

target_link_libraries(${CMAKE_PROJECT_NAME} "stdc++fs")