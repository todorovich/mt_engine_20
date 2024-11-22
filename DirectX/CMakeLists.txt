set_property(TARGET DirectX PROPERTY CXX_STANDARD 23)

target_compile_options(
	DirectX PRIVATE 
	/W1
	/wd5260
	"$<$<CONFIG:Debug>:/ZI>"
)

target_sources(DirectX
	PRIVATE
		DirectX.ixx
		d3dx12.h
)

target_include_directories(DirectX PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")