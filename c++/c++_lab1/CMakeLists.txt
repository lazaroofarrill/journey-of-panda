cmake_minimum_required(VERSION 3.20)
project(c___lab1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")


set(PANDA_INC "/usr/local/include/panda3d" "/usr/include/eigen3")
set(PANDA_LIB "/usr/local/lib/panda3d")
include_directories(${PANDA_INC})
link_directories(${PANDA_LIB})


add_executable(c___lab1 main.cpp)
target_link_libraries(c___lab1
        "p3framework"
        "panda"
        "pandafx"
        "pandaexpress"
        "p3dtoolconfig"
        "p3dtool"
        "p3direct"
        "pthread")
