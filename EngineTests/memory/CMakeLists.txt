target_sources(EngineTests
	PRIVATE
	MakeUniqueTests.ixx
	ObjectPoolTests.ixx
)

target_include_directories(EngineTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")