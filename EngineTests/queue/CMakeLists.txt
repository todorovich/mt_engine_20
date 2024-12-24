target_sources(EngineTests
	PRIVATE
	PriorityQueueTests.ixx
)

target_include_directories(EngineTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")