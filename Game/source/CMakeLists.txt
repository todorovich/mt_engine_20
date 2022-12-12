target_sources(Game
	PRIVATE
		EngineDemo.cpp
	PUBLIC
		EngineDemo.ixx
)

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")