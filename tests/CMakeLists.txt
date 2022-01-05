set(CMAKE_CXX_COMPILER "dpcpp")

set(TARGET splitter_test)
set(SOURCES "")
list(
  APPEND
  SOURCES
  splitter_test.cpp)

add_executable(${TARGET} ${SOURCES})
target_include_directories(
    ${TARGET} PUBLIC
    ../3rd_party
    ../include)

# target_link_libraries(${TARGET} PRIVATE splitter)
