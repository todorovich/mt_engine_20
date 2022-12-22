set_property(TARGET EngineTests PROPERTY CXX_STANDARD 23)
  
target_compile_options(
	EngineTests PRIVATE 
	/experimental:module
	/wd4005
	/wd5106
)

target_sources(EngineTests
	PRIVATE
		StopWatchTests.ixx
		EngineTests.ixx
		TestMain.ixx
)

target_include_directories(EngineTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")