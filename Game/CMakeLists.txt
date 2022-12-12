set_property(TARGET Game PROPERTY CXX_STANDARD 23)
  
target_compile_options(Game PRIVATE "/experimental:module")

add_subdirectory("source")
