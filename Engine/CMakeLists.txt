set_property(TARGET Engine PROPERTY CXX_STANDARD 23)

target_compile_options(
	Engine PRIVATE 
	/experimental:module
	/wd4005
	/wd5106
	#/WX
	/W4
	"$<$<CONFIG:Debug>:/ZI>"
)

target_sources(
	Engine PRIVATE
	Constants.ixx
	Engine.cpp
	Engine.ixx
	Game.ixx
)

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory("error")
add_subdirectory("font")
add_subdirectory("geometry")
add_subdirectory("input")
add_subdirectory("math")
add_subdirectory("memory")
add_subdirectory("renderer")
add_subdirectory("task")
add_subdirectory("time")
add_subdirectory("windows")
