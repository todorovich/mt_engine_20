# Install script for directory: C:/Users/micho/source/repos/mt_engine_20/Engine/source/Engine

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/micho/Source/Repos/mt_engine_20/out/install/x64-release")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/micho/source/repos/mt_engine_20/out/build/x64-release/Engine/source/Engine/command/cmake_install.cmake")
  include("C:/Users/micho/source/repos/mt_engine_20/out/build/x64-release/Engine/source/Engine/input/cmake_install.cmake")
  include("C:/Users/micho/source/repos/mt_engine_20/out/build/x64-release/Engine/source/Engine/logging/cmake_install.cmake")
  include("C:/Users/micho/source/repos/mt_engine_20/out/build/x64-release/Engine/source/Engine/renderer/cmake_install.cmake")
  include("C:/Users/micho/source/repos/mt_engine_20/out/build/x64-release/Engine/source/Engine/time/cmake_install.cmake")
  include("C:/Users/micho/source/repos/mt_engine_20/out/build/x64-release/Engine/source/Engine/windows/cmake_install.cmake")

endif()

