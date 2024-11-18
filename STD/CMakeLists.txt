set_property(TARGET STL PROPERTY CXX_STANDARD 23)

target_compile_options(
	STL PRIVATE 
	/W1
	"$<$<CONFIG:Debug>:/ZI>"
)

target_sources(STL
	PRIVATE
		std.ixx
		std.compat.ixx
)

target_include_directories(STL PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")