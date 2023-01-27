# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

# Setup vcpkg script with CMake (note: should be placed before project() call)
set(CMAKE_TOOLCHAIN_FILE c:/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

set(VCPKG_TARGET_TRIPLET x64-windows)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE 1)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "9629ab6c-6c0e-423f-bb9d-cc5ac4a22041")

project ("Engine")

find_package(freetype CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# Include sub-projects.
add_library (STL STATIC "")
target_compile_definitions(STL PRIVATE -DUNICODE -D_UNICODE -DWIN32)
add_subdirectory ("STL")

add_library (Engine STATIC "" Engine/input/BasicInputManager.cpp)
target_compile_definitions(Engine PRIVATE -DUNICODE -D_UNICODE -DWIN32)
target_link_libraries(Engine PUBLIC STL)
target_link_libraries(Engine PRIVATE freetype)
add_subdirectory ("Engine")

add_executable (EngineTests "")
target_compile_definitions(EngineTests PRIVATE UNICODE _UNICODE WIN32)
add_subdirectory ("EngineTests")
target_link_libraries(EngineTests PRIVATE Engine)
target_link_libraries(EngineTests PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_executable (Game WIN32 "")
target_compile_definitions(Game PRIVATE -DUNICODE -D_UNICODE -DWIN32)
target_link_libraries(Game PRIVATE Engine)
add_subdirectory ("Game")


include(CMakePrintHelpers)

cmake_print_variables(
	CMAKE_CXX_COMPILER_ID 
	CMAKE_CXX_COMPILER_VERSION
)

cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG_INIT)
cmake_print_variables(CMAKE_CXX_FLAGS_INIT)
cmake_print_variables(CMAKE_CXX_FLAGS_MINSIZEREL_INIT)
cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE_INIT)
cmake_print_variables(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT)
