#CMakeLists.txt

#:Author: zip76
#:Email: zip76@BugMachine2
#:Date: 2015-03-30 19:38

cmake_minimum_required(VERSION 3.2.1)

set(LIB_NAME xdrfile)
set(SRC xdrfile.c xdrfile_trr.c xdrfile_xtc.c)

# creats library
add_library(${LIB_NAME} ${SRC})

#.. vim: expandtab filetype=rst shiftwidth=4 tabstop=4

