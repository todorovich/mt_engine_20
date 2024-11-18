set_property(TARGET EngineTests PROPERTY CXX_STANDARD 23)
  
target_compile_options(
	EngineTests PRIVATE 
	/wd4005
	/wd5106
)

target_sources(EngineTests
	PRIVATE
	EngineTests.ixx
	TestMain.ixx
	MicrosoftTests.ixx
	EventTests.ixx
)

target_include_directories(EngineTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")