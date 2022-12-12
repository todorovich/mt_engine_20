# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

# Setup vcpkg script with CMake (note: should be placed before project() call)
set(CMAKE_TOOLCHAIN_FILE c:/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

set(VCPKG_TARGET_TRIPLET x64-windows)

#set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
#set(CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE 1)
#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "9629ab6c-6c0e-423f-bb9d-cc5ac4a22041")

project ("Engine")

find_package(freetype CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)


add_definitions(-DUNICODE -D_UNICODE -DWIN32)

# Include sub-projects.
add_library (Engine STATIC "")

target_link_libraries(Engine PRIVATE freetype)
#target_link_libraries(Engine PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_subdirectory ("Engine")

add_executable (Game WIN32 "")
add_subdirectory ("Game")

#Link stuff
target_link_libraries(Game PRIVATE Engine)
