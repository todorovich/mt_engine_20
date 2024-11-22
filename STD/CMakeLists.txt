set_property(TARGET STD PROPERTY CXX_STANDARD 23)

target_compile_options(
	STD PRIVATE 
	/W1
	"$<$<CONFIG:Debug>:/ZI>"
)

target_sources(STD
	PRIVATE
		std.ixx
		std.compat.ixx
)

target_include_directories(STD PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")