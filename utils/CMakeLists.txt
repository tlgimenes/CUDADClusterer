#CMakeLists.txt

#:Author: zip76
#:Email: zip76@BugMachine2
#:Date: 2015-03-30 16:24

cmake_minimum_required(VERSION 3.2.1)

set(LIB_NAME utils)
set(SRC error.hpp reader_xtc.hpp types.hpp color.hpp)

# creats library
add_library(${LIB_NAME} STATIC ${SRC})

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

#.. vim: expandtab filetype=rst shiftwidth=4 tabstop=4

