cmake_minimum_required(VERSION 3.1.0)

project(qytlib)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_subdirectory(src)
add_subdirectory(tests)
