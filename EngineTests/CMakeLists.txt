#set_property(TARGET EngineTests PROPERTY CXX_STANDARD latest)
  
target_compile_options(
	EngineTests PRIVATE 
	/std:c++latest
	/wd4005
	/wd5106
)

target_sources(EngineTests
	PRIVATE
	EngineTests.ixx
	TestMain.ixx
	MicrosoftTests.ixx
)

add_subdirectory ("event")
add_subdirectory ("input")
add_subdirectory ("memory")
add_subdirectory ("queue")

target_include_directories(EngineTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")