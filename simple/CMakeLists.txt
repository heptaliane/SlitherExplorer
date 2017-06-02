cmake_minimum_required (VERSION 2.6)

set (CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -O2")
set (CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")

set (CMAKE_BUILD_TYPE Release)

set (CMAKE_VERBOSE_MAKEFILE 1)

project (SlitherExplorer CXX)

find_package (OpenCV REQUIRED)

add_executable (
    SlitherExplorer main.cpp
    slarray.cpp
    definite.cpp
    localFiles.cpp
    search.cpp
)

target_link_libraries(SlitherExplorer ${OpenCV_LIBS})
