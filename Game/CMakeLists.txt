set_property(TARGET Game PROPERTY CXX_STANDARD 23)
  
target_compile_options(Game PRIVATE 
	/experimental:module
	/wd4005
	/wd5106
)

add_subdirectory("source")
