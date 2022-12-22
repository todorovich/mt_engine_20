set_property(TARGET STL PROPERTY CXX_STANDARD 23)

target_compile_options(
	STL PRIVATE 
	/experimental:module
	/W1
	"$<$<CONFIG:Debug>:/ZI>"
)

target_sources(STL
	PRIVATE
		STL.ixx
		d3dx12.h
)

target_include_directories(STL PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")