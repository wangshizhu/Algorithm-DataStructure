cmake_minimum_required(VERSION 3.16)

# set the project name and version
project(binary_search_tree VERSION 1.0)

file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
source_group("Header" FILES ${CURRENT_HEADERS}) 

aux_source_directory(. DIR_SRCS)
add_executable(binary_search_tree ${DIR_SRCS} ${CURRENT_HEADERS})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)