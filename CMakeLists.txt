cmake_minimum_required(VERSION 3.30)
project(lancer_console VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Build Types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

add_executable(LancerConsole main.cpp)

# Subdirectories
add_subdirectory(ConsoleIO)
add_subdirectory("Dialogue Tree")
add_subdirectory(json)
add_subdirectory(Profiles)
add_subdirectory("Text Formatting")
