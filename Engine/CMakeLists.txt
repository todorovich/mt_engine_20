set_property(TARGET Engine PROPERTY CXX_STANDARD 23)

target_compile_options(Engine PRIVATE "/experimental:module")

add_subdirectory ("source")
