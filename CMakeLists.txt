project (SimpleGL)
# set cmake minimum cmake version
cmake_minimum_required (VERSION 2.8)
# extra definitions
add_definitions(-Wall -std=c++11 -O2 -g)
# add subdirectories
add_subdirectory(lib)
add_subdirectory(editor)
