set_property(TARGET STL PROPERTY CXX_STANDARD 23)

target_compile_options(
	STL PRIVATE 
	/experimental:module
	/W1
	"$<$<CONFIG:Debug>:/ZI>"
)

add_subdirectory ("source")
