set_property(TARGET Game PROPERTY CXX_STANDARD 23)
  
target_compile_options(Game PRIVATE 
	/experimental:module
	/wd4005
	/wd5106
)

target_sources(
	Game PRIVATE
		EngineDemo.cpp
	PUBLIC
		EngineDemo.ixx
		InputHandlers.ixx
)

target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")