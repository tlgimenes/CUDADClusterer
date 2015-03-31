#CMakeLists.txt

#:Author: LOBATO GIMENES, Tiago
#:Email: tlgimenes@gmail.com
#:Date: 2015-03-30 14:54

set(PROJECT_NAME gpu_clusterer)
set(SUB_DIRS parser utils xdrfile)
set(SUB_DIRS_LIBS ${SUB_DIRS})
set(CFLAGS -std=c++11)
set(ADD_LIBS -lboost_system -lboost_filesystem)

# Minimum cmake version required to build the project
cmake_minimum_required(VERSION 3.2.1)

# Support C++11 necessary
add_definitions(${CFLAGS})

# Name of the projet
project(${PROJECT_NAME})

# Adds subdirectories to the project
#add_subdirectory(xdrfile)
add_subdirectory(xdrfile)
add_subdirectory(parser)
add_subdirectory(utils)

# Adds subdirectories to the project
include_directories(${SUB_DIRS})

# adds an executable for the project
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SUB_DIRS_LIBS} ${ADD_LIBS})

#.. vim: expandtab filetype=rst shiftwidth=4 tabstop=4

