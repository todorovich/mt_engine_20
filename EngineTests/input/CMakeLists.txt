target_sources(EngineTests
	PRIVATE
	InputManagerTests.ixx
	InputTestEngine.ixx
)

target_include_directories(EngineTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")