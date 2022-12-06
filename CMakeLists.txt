# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.13)

project ("Engine")

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")

  file(DOWNLOAD 
    "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
    TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(
    CONANFILE conanfile.txt  # or relative build/conanfile.txt
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
)

# Include sub-projects.
add_subdirectory ("Engine")
add_subdirectory ("Game")