add_executable (Game "")

set_property(TARGET Game PROPERTY CXX_STANDARD 20)

target_link_libraries(Game Engine)

add_subdirectory("source")