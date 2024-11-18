# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

project ("Engine")

set(VCPKG_TARGET_TRIPLET x64-windows)
set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_SCAN_FOR_MODULES 1)

find_package(freetype CONFIG REQUIRED)
find_package(xxHash CONFIG REQUIRED)

add_library (STL STATIC "")
target_compile_definitions(STL PRIVATE -DUNICODE -D_UNICODE -DWIN32)

add_library (Engine STATIC "")
target_compile_definitions(Engine PRIVATE -DUNICODE -D_UNICODE -DWIN32)

target_link_libraries(Engine PUBLIC STL)
target_link_libraries(Engine PRIVATE Freetype::Freetype)
target_link_libraries(Engine PRIVATE xxHash::xxhash)

find_package(Catch2 CONFIG REQUIRED)

add_executable (EngineTests "")
target_compile_definitions(EngineTests PRIVATE UNICODE _UNICODE WIN32)

target_link_libraries(EngineTests PRIVATE Engine)
target_link_libraries(EngineTests PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_executable (Game WIN32 "")
target_compile_definitions(Game PRIVATE -DUNICODE -D_UNICODE -DWIN32)

target_link_libraries(Game PRIVATE Engine)

add_subdirectory ("Engine")
add_subdirectory ("STD")
add_subdirectory ("EngineTests")
add_subdirectory ("Game")

include(CMakePrintHelpers)
cmake_print_variables(
	CMAKE_CXX_COMPILER_ID 
	CMAKE_CXX_COMPILER_VERSION
)
