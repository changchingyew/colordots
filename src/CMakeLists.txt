set(CMAKE_CXX_COMPILER "dpcpp")

set(CMAKE_POSITION_INDEPENDENT_CODE true)
add_library(splitter STATIC)

target_sources(
    splitter
    PRIVATE splitter.cpp)

target_include_directories(
    splitter
    PUBLIC ../include)