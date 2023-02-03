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

message("CMAKE_C_FLAGS_DEBUG is ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE is ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO is ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL is ${CMAKE_C_FLAGS_MINSIZEREL}")

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory("error")
add_subdirectory("font")
add_subdirectory("geometry")
add_subdirectory("input")
add_subdirectory("math")
add_subdirectory("memory")
add_subdirectory("gsl")
add_subdirectory("renderer")
add_subdirectory("task")
add_subdirectory("time")
add_subdirectory("windows")
