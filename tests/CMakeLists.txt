set(CMAKE_CXX_COMPILER "dpcpp")

set(TARGET splitter_test)
set(SOURCES "")
list(
  APPEND
  SOURCES
  splitter_test.cpp)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE splitter)
