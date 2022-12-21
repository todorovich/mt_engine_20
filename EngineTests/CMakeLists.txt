set_property(TARGET EngineTests PROPERTY CXX_STANDARD 23)
  
target_compile_options(
	EngineTests PRIVATE 
	/experimental:module
	/wd4005
	/wd5106
)

add_subdirectory("source")