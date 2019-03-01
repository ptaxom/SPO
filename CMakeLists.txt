cmake_minimum_required(VERSION 3.0.0)
project(labSPO1 VERSION 0.1.0)

include(CTest)
enable_testing()


set(SRC_C src/child)
set(SRC_P src/parent)


set(CHILD_SOURCES ${SRC_C}/child.cpp)
set(PARENT_SOURCES ${SRC_P}/parent.cpp)


add_executable(parent ${PARENT_SOURCES})
add_executable(child  ${CHILD_SOURCES})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
