#set_property(TARGET Engine PROPERTY CXX_STANDARD latest)

target_compile_options(
	Engine PRIVATE 
	/std:c++latest
	/wd4005
	/wd5106
	/W4
	"$<$<CONFIG:Debug>:/ZI>"
)

target_sources(
	Engine PRIVATE
	Constants.ixx
	Engine.ixx
	Engine.cpp
	Game.ixx
	StandardEngine.ixx
	StandardEngine.cpp
)

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory("error")
add_subdirectory("event")
add_subdirectory("font")
add_subdirectory("geometry")
add_subdirectory("input")
add_subdirectory("math")
add_subdirectory("memory")
add_subdirectory("gsl")
add_subdirectory("renderer")
add_subdirectory("queue")
add_subdirectory("task")
add_subdirectory("time")
add_subdirectory("utility")
add_subdirectory("windows")
