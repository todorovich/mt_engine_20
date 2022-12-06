add_library (Engine "")

set_property(TARGET Engine PROPERTY CXX_STANDARD 23)
  
add_definitions(-DUNICODE -D_UNICODE)

target_compile_options(Engine PRIVATE "/experimental:module")

target_include_directories(Engine PRIVATE ${CONAN_INCLUDE_DIRS_FREETYPE})
target_link_libraries(Engine PUBLIC ${CONAN_LIBS})

add_subdirectory ("source")