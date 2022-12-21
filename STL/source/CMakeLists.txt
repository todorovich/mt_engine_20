target_sources(STL
	PRIVATE
		STL.ixx
		d3dx12.h
)

target_include_directories(STL PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")